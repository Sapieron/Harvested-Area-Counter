/*
 * main.c
 *
 *  Created on: 29.07.2019
 *      Author: Pawe³ Klisz @ pawelochojec@gmail.com
 *
 *      This code is designed to be used on STM32F103C8T6.
 *
 *      Application uses 3 external hall sensors to acquire position of the wheel on a harvester.
 *      By knowing the width of harvester's collector, the area of material collected can be easily determined.
 *      Once a full spin is done number on a display gets updated.
 *
 */

#include <stm32f103c8t6.h>
#include <stm32f103c8t6_gpio_driver.h>
#include <stm32f103c8t6_timer_driver.h>
#include <string.h>
#include <stddef.h>


/*
 * B10	- segA
 * B11	- segB
 * B14	- segC
 * B13	- segD
 * B5	- segE
 * B1	- segF
 * B0	- segG
 * B9	- dp


 *
 * B6	-	digit msb
 * B7	-	digit 2
 * B8	-	digit 1
 * B15	-	digit lsb
 */

/*
 * A10 - sensor 1
 * A11 - sensor 2
 * A12 - sensor 3
 *
 * B12 - reset button
 */

/*********************************************************************************************************
 * ****************************************** USER DEFINES ***********************************************
 *********************************************************************************************************/

#define numOfDigits 3		//Change this number, if your display has 1-4 digits

#define ONE_SPIN	1885	// 0x1 is 0,01 m^2. EXAMPLE: 1885 means 18,85m^2

/*********************************************************************************************************
 * **************************************Generic macros and defines***************************************
 *********************************************************************************************************/



const uint8_t pinout[8] = {10,11,14,13,5,1,0,9};

const uint8_t digitPins[] = {6, 7, 8, 15};

const uint8_t sensorPins[] = {10, 11, 12};

GPIO_Handle_t digitHandler[numOfDigits];

GPIO_Handle_t sensorHandler[3];

GPIO_Handle_t digit;

TIMER2_5_Handle_t timer3Handle;

uint16_t data=0;

uint16_t result[numOfDigits] = {0};

uint8_t temp = 4, var=0;


// 011X110XXX1XXX10b results in 0 shown on 7seg display - where X stand for ,,always zero"

const uint16_t numbers[11] =
{
		0x6C22,
		0x4800,
		0x2C21,
		0x6C01,
		0x4803,
		0x6403,
		0x6423,
		0x4C00,
		0x6C23,
		0x6C03,
		0x0200
};
void DATA_Handler(uint32_t dataToConvert);

void SENSOR_Handling(uint8_t sensorMask);


/****************************************************************************************************
 * ************************************* MAIN FUNCTION **********************************************
 ****************************************************************************************************/



int main()
{
	TIMER2_5_Handle_t timer2Handle;


	/************************************************************************************************
	 ******************************* Clear structures with 0's***************************************
	 ************************************************************************************************/

	memset(&digit,0,sizeof(digit));

	memset(&timer2Handle, 0, sizeof(timer2Handle));
	memset(&timer3Handle, 0, sizeof(timer3Handle));


	/*************************************************************************************************
	 ********************************* Set structures constants***************************************
	 *************************************************************************************************/


	for(int8_t i = 0; i < numOfDigits; ++i )
	{
		memset(&digitHandler[i],0,sizeof(digitHandler[i]));
		digitHandler[i].pGPIOx = GPIOB;
		digitHandler[i].PinConfig.PinInOrOut = GPIO_PIN_OUTPUT_PP;
		digitHandler[i].PinConfig.PinMode = GPIO_PIN_MODE_OUTPUT2MHZ;
		digitHandler[i].PinConfig.PinNumber = digitPins[i];
	}



	for(int8_t i = 0; i < (sizeof(sensorHandler)/sizeof(GPIO_Handle_t)); ++i )
	{
		memset(&sensorHandler[i],0,sizeof(sensorHandler[i]));
		sensorHandler[i].pGPIOx = GPIOA;
		sensorHandler[i].PinConfig.PinNumber = sensorPins[i];
		sensorHandler[i].PinConfig.PinInOrOut = GPIO_PIN_INPUT_W_PUPD;
		sensorHandler[i].PinConfig.PinMode = GPIO_PIN_MODE_INPUT;
	}



	for(uint8_t i =0; i < (sizeof(numbers)/sizeof(uint16_t)); ++i){
		digit.pGPIOx = GPIOB;
		digit.PinConfig.PinInOrOut = GPIO_PIN_OUTPUT_PP;
		digit.PinConfig.PinMode = GPIO_PIN_MODE_OUTPUT2MHZ;
		digit.PinConfig.PinNumber = pinout[i];
		GPIO_Init(&digit);
		GPIO_WriteToOutput(&digit,pinout[i],RESET_PNP);
	}

	timer2Handle.pTIMERreg = TIMER2;
	timer2Handle.Config.autoReloadVal =	100-1;					//IRQ generation freq. of timer is 1kHz
	timer2Handle.Config.prescalerVal = F_CPU / 100000-1;		//WARNING: this can't exceed 16bit wideness
	timer2Handle.Config.initialize = ENABLE;
	timer2Handle.Config.interruptEnable = ENABLE;

	timer3Handle.pTIMERreg = TIMER3;
	timer3Handle.Config.autoReloadVal =	2000-1;				//200ms
	timer3Handle.Config.prescalerVal = F_CPU / 100000-1;
	timer3Handle.Config.initialize = ENABLE;
	timer3Handle.Config.interruptEnable = ENABLE;


	/**************************************************************************************************
	 ********************** Initialize clocks and registers with structure constants********************
	 **************************************************************************************************/

	//Initialize sensor inputs

	for(int8_t i = 0; i < (sizeof(sensorHandler)/sizeof(GPIO_Handle_t)); ++i )
	{
		GPIO_Init(&sensorHandler[i]);
		GPIO_WriteToOutput(&sensorHandler[i],sensorPins[i],ENABLE);		//it is required to enable pullup in input mode
	}



	for(int8_t i = 0; i < numOfDigits; ++i )
	{
		GPIO_Init(&digitHandler[i]);
		GPIO_WriteToOutput(&digitHandler[i],digitPins[i],RESET_PNP);
	}


	//setup two timers - timer2 is used to generate interrupts of LED display, timer3 is used in delay function
	TIM2_5_Init(&timer2Handle, ENABLE);
	TIM2_5_Init(&timer3Handle, DISABLE);

	IRQ_Config(IRQ_TIMER2, ENABLE);

	IRQ_Config(IRQ_EXTI15_10, ENABLE);

	//setup interrupts for sensors
	for(int8_t i = 0; i < (sizeof(sensorHandler)/sizeof(GPIO_Handle_t)); ++i )
	{
		GPIO_IRQ_EXTI_Init(EXTI_POINTER ,EXTI_IMR_FTSR, sensorPins[i]);
	}

	//setup interrupt for timer3 - it is used as delay functions in later part of this code
	IRQ_Config(IRQ_TIMER3, ENABLE);


	/**************************************************************************************************
	 * ****************************************** Main loop *******************************************
	 **************************************************************************************************/
	while(1);

}



void TIM3_IRQHandler(void)
{
	if(TIMER3->SR & (1 << TIMER2_5_SR_UIF ))
	{
		TIMER3->SR &= ~(1 << TIMER2_5_SR_UIF);
		TimerHandling(&timer3Handle);
		if(var==4)
		{
			var=1;
			data++;
		}

	}
}



void EXTI15_10_IRQHandler(void)
{
	SensorHandling(&timer3Handle);
	if(EXTI_POINTER->PR & (ENABLE << sensorPins[0]))
	{
		GPIO_IRQHandling(EXTI_POINTER, sensorPins[0]);
		SENSOR_Handling(1);
	}
	else if(EXTI_POINTER->PR & (ENABLE << sensorPins[1]))
	{
		GPIO_IRQHandling(EXTI_POINTER, sensorPins[1]);
		SENSOR_Handling(2);
	}
	else if(EXTI_POINTER->PR & (ENABLE << sensorPins[2]))
	{
		GPIO_IRQHandling(EXTI_POINTER, sensorPins[2]);
		SENSOR_Handling(4);
	}
}



void TIM2_IRQHandler()
{
	if(TIMER2->SR & (1 << TIMER2_5_SR_UIF ))
	{
		TIMER2->SR &= ~(1 << TIMER2_5_SR_UIF);	//check update interrupt flag, then clear it
		DATA_Handler(data*ONE_SPIN/1000);
		SSEG_SetDigit(&digitHandler[0], (const uint8_t)numOfDigits, result);
	}
}




void DATA_Handler(uint32_t dataToConvert)
{

	for(int16_t i = (numOfDigits-1); i >= 0 ; --i)		//signed variable is required, as after
													//last cycle ,,i" could be set to 0xFFFF, resulting in infinite loop
	{
		if(i == (numOfDigits-2))
		{
			result[i] = (numbers[dataToConvert%10] | numbers[10]);
			dataToConvert=dataToConvert/10;
			continue;
		}
		result[i] = numbers[dataToConvert%10];
		dataToConvert=dataToConvert/10;
	}
}



void SENSOR_Handling(uint8_t sensorMask)
{
	if(sensorMask == 1)
	{
		if(temp == 4)
		{
			var++;
			temp=sensorMask;
		}

	}else if(sensorMask == 2)
	{
		if(temp == 1)
		{
			var++;
			temp=sensorMask;
		}
	}else if(sensorMask == 4)
	{
		if(temp == 2)
		{
			var++;
			temp=sensorMask;
		}
	}
}

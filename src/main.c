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
 * B7	-	digit middle
 * B8	-	digit lsb
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

#define digit1pin		6
#define digit2pin		7
#define digit3pin		8




GPIO_Handle_t DIGIT1Handler, DIGIT2Handler, DIGIT3Handler, DIGIT4Handler;			//TODO dodac obsluge dwoch kolejnych cyfr

GPIO_Handle_t digit, sensor1, sensor2, sensor3;

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

/****************************************************************************************************
 * ************************************* MAIN FUNCTION **********************************************
 ****************************************************************************************************/



int main()
{
	TIMER2_5_Handle_t timer2Handle;


	/************************************************************************************************
	 ******************************* Clear structures with 0's***************************************
	 ************************************************************************************************/
	memset(&DIGIT1Handler,0,sizeof(DIGIT1Handler));
	memset(&DIGIT2Handler,0,sizeof(DIGIT2Handler));
	memset(&DIGIT3Handler,0,sizeof(DIGIT3Handler));

	memset(&sensor1,0,sizeof(sensor1));
	memset(&digit,0,sizeof(digit));

	memset(&timer2Handle, 0, sizeof(timer2Handle));
	memset(&timer3Handle, 0, sizeof(timer3Handle));


	/*************************************************************************************************
	 ********************************* Set structures constants***************************************
	 *************************************************************************************************/

	DIGIT1Handler.pGPIOx = GPIOB;				//TODO to tez wpuscic w ³adna petle
	DIGIT1Handler.PinConfig.PinInOrOut = GPIO_PIN_OUTPUT_PP;
	DIGIT1Handler.PinConfig.PinMode = GPIO_PIN_MODE_OUTPUT2MHZ;
	DIGIT1Handler.PinConfig.PinNumber = digit1pin;

	DIGIT2Handler.pGPIOx = GPIOB;
	DIGIT2Handler.PinConfig.PinInOrOut = GPIO_PIN_OUTPUT_PP;
	DIGIT2Handler.PinConfig.PinMode = GPIO_PIN_MODE_OUTPUT2MHZ;
	DIGIT2Handler.PinConfig.PinNumber = digit2pin;

	DIGIT3Handler.pGPIOx = GPIOB;
	DIGIT3Handler.PinConfig.PinInOrOut = GPIO_PIN_OUTPUT_PP;
	DIGIT3Handler.PinConfig.PinMode = GPIO_PIN_MODE_OUTPUT2MHZ;
	DIGIT3Handler.PinConfig.PinNumber = digit3pin;

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

	sensor1.pGPIOx = GPIOA;
	sensor1.PinConfig.PinNumber = GPIO_PIN_NUMBER_10;
	sensor1.PinConfig.PinInOrOut = GPIO_PIN_INPUT_W_PUPD;
	sensor1.PinConfig.PinMode = GPIO_PIN_MODE_INPUT;

	sensor2.pGPIOx = GPIOA;
	sensor2.PinConfig.PinNumber = GPIO_PIN_NUMBER_11;
	sensor2.PinConfig.PinInOrOut = GPIO_PIN_INPUT_W_PUPD;
	sensor2.PinConfig.PinMode = GPIO_PIN_MODE_INPUT;

	sensor3.pGPIOx = GPIOA;
	sensor3.PinConfig.PinNumber = GPIO_PIN_NUMBER_12;
	sensor3.PinConfig.PinInOrOut = GPIO_PIN_INPUT_W_PUPD;
	sensor3.PinConfig.PinMode = GPIO_PIN_MODE_INPUT;


	for(uint8_t i =0; i < (sizeof(numbers)/sizeof(uint16_t)); ++i){
		digit.pGPIOx = GPIOB;
		digit.PinConfig.PinInOrOut = GPIO_PIN_OUTPUT_PP;
		digit.PinConfig.PinMode = GPIO_PIN_MODE_OUTPUT2MHZ;
		digit.PinConfig.PinNumber = pinout[i];
		GPIO_Init(&digit);
		GPIO_WriteToOutput(&digit,pinout[i],RESET_PNP);
	}

	/**************************************************************************************************
	 ********************** Initialize clocks and regiters with structure constants********************
	 **************************************************************************************************/


	//Initialize sensor inputs
	GPIO_Init(&sensor1);
	GPIO_Init(&sensor2);
	GPIO_Init(&sensor3);

	GPIO_WriteToOutput(&sensor1,GPIO_PIN_NUMBER_10,ENABLE);		//it is required to enable pullup in input mode
	GPIO_WriteToOutput(&sensor2,GPIO_PIN_NUMBER_11,ENABLE);		//it is required to enable pullup in input mode
	GPIO_WriteToOutput(&sensor3,GPIO_PIN_NUMBER_12,ENABLE);		//it is required to enable pullup in input mode




	GPIO_Init(&DIGIT1Handler);
	GPIO_WriteToOutput(&DIGIT1Handler,GPIO_PIN_NUMBER_6,RESET_PNP);
	if(numOfDigits<3)
	{
		GPIO_Init(&DIGIT2Handler);
		GPIO_WriteToOutput(&DIGIT2Handler,GPIO_PIN_NUMBER_7,RESET_PNP);
	}
	if(numOfDigits<4)
	{
		GPIO_Init(&DIGIT3Handler);
		GPIO_WriteToOutput(&DIGIT3Handler,GPIO_PIN_NUMBER_8,RESET_PNP);
	}
	if(numOfDigits<5)
	{
		GPIO_Init(&DIGIT4Handler);
		GPIO_WriteToOutput(&DIGIT4Handler,GPIO_PIN_NUMBER_9,RESET_PNP);		//TODO sprawdzic ktory pin jest wolny
	}



	//setup two timer - timer2 is used to generate interrupts of LED display, timer3 is used in delay function
	TIM2_5_Init(&timer2Handle, ENABLE);
	TIM2_5_Init(&timer3Handle, DISABLE);

	IRQ_Config(IRQ_TIMER2, ENABLE);

	IRQ_Config(IRQ_EXTI15_10, ENABLE);

	//setup interrupts for sensors
	GPIO_IRQ_EXTI_Init(EXTI_POINTER ,EXTI_IMR_FTSR, GPIO_PIN_NUMBER_10);
	GPIO_IRQ_EXTI_Init(EXTI_POINTER ,EXTI_IMR_FTSR, GPIO_PIN_NUMBER_11);
	GPIO_IRQ_EXTI_Init(EXTI_POINTER ,EXTI_IMR_FTSR, GPIO_PIN_NUMBER_12);

	//setup interrupt for timer3 - it is used as delay functions in later part of this code
	IRQ_Config(IRQ_TIMER3, ENABLE);
	//data--;

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
	if(EXTI_POINTER->PR & (ENABLE << GPIO_PIN_NUMBER_10))
	{
		GPIO_IRQHandling(EXTI_POINTER, GPIO_PIN_NUMBER_10);
		SENSOR_Handling(1);
	}
	else if(EXTI_POINTER->PR & (ENABLE << GPIO_PIN_NUMBER_11))
	{
		GPIO_IRQHandling(EXTI_POINTER, GPIO_PIN_NUMBER_11);
		SENSOR_Handling(2);
	}
	else if(EXTI_POINTER->PR & (ENABLE << GPIO_PIN_NUMBER_12))
	{
		GPIO_IRQHandling(EXTI_POINTER, GPIO_PIN_NUMBER_12);
		SENSOR_Handling(4);
	}
}


void TIM2_IRQHandler()
{
	if(TIMER2->SR & (1 << TIMER2_5_SR_UIF ))
	{
		TIMER2->SR &= ~(1 << TIMER2_5_SR_UIF);	//check update interrupt flag, then clear it
		DATA_Handler(data*ONE_SPIN/1000);
		SSEG_SetDigit(&DIGIT1Handler, (const uint8_t)numOfDigits, result);
	}
}



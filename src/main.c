/*
 * main.c
 *
 *  Created on: 29.07.2019
 *      Author: Pawe³ Klisz @ pawelochojec@gmail.com
 *
 *      Repository for this project:
 *      https://github.com/Sapieron/combine-harvester-STM32F103C8T6
 *      Always check if there's a newer version available.
 *
 *      This code is designed to be used on STM32F103C8T6.
 *
 *      Application uses 3 external hall sensors to acquire position of the wheel on a harvester.
 *      By knowing the width of harvester's collector, the area of material collected can be easily determined.
 *      Once a full spin is done number on a display gets updated.
 *
 *      Extra information, as well as pinoout is available in README file on my github for this project
 *
 */

#include <string.h>
#include <stddef.h>
#include <stm32f103c8t6.h>
#include <stm32f103c8t6_gpio.h>
#include <stm32f103c8t6_rcc.h>
#include <stm32f103c8t6_interrupt.h>
#include <stm32f103c8t6_timer.h>

/*
 * short brief of variables below:
 *
 * NUM_OF_DIGITS_ON_DISPLAY - accepts numbers from 0x1 to 0x4. It stores the number of digits used in setup
 *
 * ONE_SPIN - stores the value used to calculate how much square meters were cropped
 * once a full spin of harvester was done. This variable can be calculated as follows:
 * 1 [decimal] stands for 0,01 m^2. EXAMPLE: 1885[decimal] means 18,85m^2 will be counted once a full spin was done.
 */

#define NUM_OF_DIGITS_ON_DISPLAY 	3
#define ONE_SPIN					1885
#define POSITION_OF_DOT				2

GPIO_Handle_t digitHandler[NUM_OF_DIGITS_ON_DISPLAY];
GPIO_Handle_t sensorsHandler[3];
GPIO_Handle_t segmentOfDigit;

TIMER2_5_Handle_t timer2Handle;
TIMER2_5_Handle_t timer3Handle;

uint16_t fullSpinsMade = 0;
uint16_t resultToShowOnDisplay[NUM_OF_DIGITS_ON_DISPLAY] = {0};
uint8_t initialSensorsStatus = 4, correctOrderOfSwitchingSensors = 0;

const uint8_t sensorMask[3] = {1,2,4};
const uint8_t pinToSegment[] = {10,11,14,13,5,1,0,9};
const uint8_t digitPins[] = {6, 7, 8, 15};
const uint8_t sensorPins[] = {10, 11, 12};
const uint16_t numberToBitMask[11] = {
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

void DataHandler(uint32_t dataToConvert);
void SensorHandler(uint8_t mask);

void InitializeDigitHandlers();
void InitializeSensorHandlers();
void InitializeSegmentOfDigitHandlers();
void SensorsEnablePullups();
void InitializeSensorPinsIRQs();
void ClearHandlers();
void InitializeTimerHandlers();
void Setup();

int main(){
	Setup();
	while(1);
}

void Setup(){
	ClearHandlers();

	InitializeTimerHandlers();
	InitializeDigitHandlers();
	InitializeSensorHandlers();
	InitializeSegmentOfDigitHandlers();
	SensorsEnablePullups();

	TIM2_5_Init(&timer2Handle, ENABLE);
	TIM2_5_Init(&timer3Handle, DISABLE);

	IRQ_IRQNumberHandling(IRQ_TIMER2, ENABLE);
	IRQ_IRQNumberHandling(IRQ_EXTI15_10, ENABLE);

	InitializeSensorPinsIRQs();

	IRQ_IRQNumberHandling(IRQ_TIMER3, ENABLE);
}

void ClearHandlers(){
	memset(&segmentOfDigit,0,sizeof(segmentOfDigit));
	memset(&timer2Handle, 0, sizeof(timer2Handle));
	memset(&timer3Handle, 0, sizeof(timer3Handle));
}

void InitializeTimerHandlers(){
	timer2Handle.pTIMERreg = TIMER2;
	timer2Handle.Config.autoReloadVal =	100-1;
	timer2Handle.Config.prescalerVal = F_CPU / 100000-1;
	timer2Handle.Config.initialize = ENABLE;
	timer2Handle.Config.interruptEnable = ENABLE;

	timer3Handle.pTIMERreg = TIMER3;
	timer3Handle.Config.autoReloadVal =	2000-1;
	timer3Handle.Config.prescalerVal = F_CPU / 100000-1;
	timer3Handle.Config.initialize = ENABLE;
	timer3Handle.Config.interruptEnable = ENABLE;
}

void InitializeDigitHandlers(){
	for(int8_t i = 0; i < NUM_OF_DIGITS_ON_DISPLAY; ++i ){
		memset(&digitHandler[i],0,sizeof(digitHandler[i]));
		digitHandler[i].pGPIOx = GPIOB;
		digitHandler[i].PinConfig.PinNumber = digitPins[i];
		digitHandler[i].PinConfig.PinInOrOut = GPIO_PIN_OUTPUT_PP;
		digitHandler[i].PinConfig.PinMode = GPIO_PIN_MODE_OUTPUT2MHZ;
		GPIO_WriteToOutput(&digitHandler[i],digitPins[i],RESET_PNP);
	}
}

void InitializeSensorHandlers(){
	for(int8_t i = 0; i < (sizeof(sensorsHandler)/sizeof(GPIO_Handle_t)); ++i ){
		memset(&sensorsHandler[i],0,sizeof(sensorsHandler[i]));
		sensorsHandler[i].pGPIOx = GPIOA;
		sensorsHandler[i].PinConfig.PinNumber = sensorPins[i];
		sensorsHandler[i].PinConfig.PinInOrOut = GPIO_PIN_INPUT_W_PUPD;
		sensorsHandler[i].PinConfig.PinMode = GPIO_PIN_MODE_INPUT;
	}
}

void InitializeSegmentOfDigitHandlers(){
	for(uint8_t i =0; i < (sizeof(numberToBitMask)/sizeof(uint16_t)); ++i){
		segmentOfDigit.pGPIOx = GPIOB;
		segmentOfDigit.PinConfig.PinInOrOut = GPIO_PIN_OUTPUT_PP;
		segmentOfDigit.PinConfig.PinMode = GPIO_PIN_MODE_OUTPUT2MHZ;
		segmentOfDigit.PinConfig.PinNumber = pinToSegment[i];
		GPIO_Init(&segmentOfDigit);
		GPIO_WriteToOutput(&segmentOfDigit,pinToSegment[i],RESET_PNP);
	}
}

void SensorsEnablePullups(){
	for(int8_t i = 0; i < (sizeof(sensorsHandler)/sizeof(GPIO_Handle_t)); ++i ){
		GPIO_Init(&sensorsHandler[i]);
		GPIO_WriteToOutput(&sensorsHandler[i],sensorPins[i],ENABLE_PIN_PULLUP);
	}
}

void InitializeSensorPinsIRQs(){
	for(int8_t i = 0; i < (sizeof(sensorsHandler)/sizeof(GPIO_Handle_t)); ++i ){
		GPIO_IRQ_EXTI_Init(EXTI_POINTER ,EXTI_IMR_FTSR, sensorPins[i]);
	}
}


void TIM3_IRQHandler(void){
	if(isTimerFlagSet(&timer3Handle)){
		ClearFlagStatus(&timer3Handle);
		Timer2_5CounterDisable(&timer3Handle);
		if(correctOrderOfSwitchingSensors==4){
			correctOrderOfSwitchingSensors=1;
			fullSpinsMade++;
		}
	}
}

void EXTI15_10_IRQHandler(void){
	SensorHandling(&timer3Handle);
	if(EXTI_POINTER->PR & (ENABLE << sensorPins[0])){
		IRQ_PendingPinInterrupt(EXTI_POINTER, sensorPins[0]);
		SensorHandler(sensorMask[0]);
	}else if(EXTI_POINTER->PR & (ENABLE << sensorPins[1])){
		IRQ_PendingPinInterrupt(EXTI_POINTER, sensorPins[1]);
		SensorHandler(sensorMask[1]);
	}else if(EXTI_POINTER->PR & (ENABLE << sensorPins[2])){
		IRQ_PendingPinInterrupt(EXTI_POINTER, sensorPins[2]);
		SensorHandler(sensorMask[2]);
	}
}

void TIM2_IRQHandler(){
	if(isTimerFlagSet(&timer2Handle)){
		ClearFlagStatus(&timer2Handle);
		DataHandler(fullSpinsMade*ONE_SPIN/1000);
		SSEG_SetDigit(&digitHandler[0], (const uint8_t)NUM_OF_DIGITS_ON_DISPLAY, resultToShowOnDisplay);
	}
}

void DataHandler(uint32_t dataToConvert){
	for(int16_t i = (NUM_OF_DIGITS_ON_DISPLAY-1); i >= 0 ; --i){
		if(i == (NUM_OF_DIGITS_ON_DISPLAY-POSITION_OF_DOT)){
			resultToShowOnDisplay[i] = (numberToBitMask[dataToConvert%10] | numberToBitMask[10]);
			dataToConvert=dataToConvert/10;
			continue;
		}
		resultToShowOnDisplay[i] = numberToBitMask[dataToConvert%10];
		dataToConvert=dataToConvert/10;
	}
}

void SensorHandler(uint8_t mask){
	if(mask == sensorMask[0]){
		if(initialSensorsStatus == sensorMask[2]){
			correctOrderOfSwitchingSensors++;
			initialSensorsStatus=mask;
		}
	}else if(mask == sensorMask[1]){
		if(initialSensorsStatus == sensorMask[0]){
			correctOrderOfSwitchingSensors++;
			initialSensorsStatus=mask;
		}
	}else if(mask == sensorMask[2]){
		if(initialSensorsStatus == sensorMask[1]){
			correctOrderOfSwitchingSensors++;
			initialSensorsStatus=mask;
		}
	}
}

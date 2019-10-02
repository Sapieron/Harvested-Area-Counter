/*
 * stm32f103c8t6_gpio_driver.h
 *
 *  Created on: 29.07.2019
 *      Author: Pawel
 */

#ifndef INC_STM32F103C8T6_GPIO_H_
#define INC_STM32F103C8T6_GPIO_H_

#include <stm32f103c8t6.h>

/******************************************************************************************
 ********************************* Typedef structures *************************************
 ******************************************************************************************/

typedef struct{
	uint32_t PinInOrOut;
	uint32_t PinMode;
	uint32_t PinNumber;
}GPIO_PinConfig_t;

typedef struct{
	GPIO_RegDef_t 	*pGPIOx;
	GPIO_PinConfig_t PinConfig;
}GPIO_Handle_t;


/******************************************************************************************
 ********************** Main functions definitions ****************************************
 ******************************************************************************************/

void GPIO_Init(GPIO_Handle_t *pGPIOHandle);

void GPIO_WriteToOutput(GPIO_Handle_t *pGPIOHandle, uint8_t pinNumber, uint8_t enOrDis);

void SSEG_SetDigit(GPIO_Handle_t *pGPIOHandle, const uint8_t numOfDigits, const uint16_t *displayedNumber);

void GPIO_IRQ_EXTI_Init(EXTI_RegDef_t *pEXTI ,uint8_t EXTItriggerType, uint8_t PinNumber);

uint8_t GPIO_ReadFromInput(GPIO_Handle_t *pGPIOHandle, uint8_t PinNumber);

void GPIO_ToggleOutputPin(GPIO_Handle_t *pGPIOHandle, uint8_t pinNumber);

void IRQ_PendingPinInterrupt(EXTI_RegDef_t *pEXTI,uint8_t PinNumber);


/******************************************************************************************
 ********************************* GPIO Specific defines **********************************
 ******************************************************************************************/



#define NUMBER_OF_DIGITS_BITMASK(numOfDigitsnDisplay)	((numOfDigitsnDisplay==0)?0:\
												(numOfDigitsnDisplay==1)?1:\
												(numOfDigitsnDisplay==2)?3:\
												(numOfDigitsnDisplay==3)?7:\
												(numOfDigitsnDisplay==4)?15:0)

#define CLEAN_SEGMENTS_BITMASK 					0x6E23

#define GPIO_PIN_MODE_INPUT			0
#define GPIO_PIN_MODE_OUTPUT10MHZ	1
#define GPIO_PIN_MODE_OUTPUT2MHZ	2
#define GPIO_PIN_MODE_OUTPUT50MHZ	3


#define GPIO_PIN_INPUT_ANALOG_MODE	0
#define GPIO_PIN_INPUT_FLOATING		1
#define GPIO_PIN_INPUT_W_PUPD		2


#define GPIO_PIN_OUTPUT_PP			0
#define GPIO_PIN_OUTPUT_OD			1
#define GPIO_PIN_OUTPUT_AF_PP		2
#define GPIO_PIN_OUTPUT_AF_OD		3


#define GPIO_PIN_NUMBER_0			0
#define GPIO_PIN_NUMBER_1			1
#define GPIO_PIN_NUMBER_2			2
#define GPIO_PIN_NUMBER_3			3
#define GPIO_PIN_NUMBER_4			4
#define GPIO_PIN_NUMBER_5			5
#define GPIO_PIN_NUMBER_6			6
#define GPIO_PIN_NUMBER_7			7
#define GPIO_PIN_NUMBER_8			8
#define GPIO_PIN_NUMBER_9			9
#define GPIO_PIN_NUMBER_10			10
#define GPIO_PIN_NUMBER_11			11
#define GPIO_PIN_NUMBER_12			12
#define GPIO_PIN_NUMBER_13			13
#define GPIO_PIN_NUMBER_14			14
#define GPIO_PIN_NUMBER_15			15




#endif /* INC_STM32F103C8T6_GPIO_H_ */

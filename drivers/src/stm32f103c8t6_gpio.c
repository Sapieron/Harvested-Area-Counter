/*
 * stm32f103c8t6_gpio_driver.c
 *
 *  Created on: 29.07.2019
 *      Author: Pawel
 */

#include <stdint.h>
#include <stm32f103c8t6.h>
#include <stm32f103c8t6_gpio.h>
#include <stm32f103c8t6_rcc.h>

/******************************************************************************************
 ********************** Private functions and defines *************************************
 ******************************************************************************************/
static uint16_t digitsPNPTransistorBitmask[4] = {1, 2, 4, 512};

static uint8_t chosenSegmentNum = 0;

static void GPIOSetMode(GPIO_Handle_t *pGPIOHandle){
	if(pGPIOHandle->PinConfig.PinNumber < 8){
		pGPIOHandle->pGPIOx->CRL &= ~(0x3 << pGPIOHandle->PinConfig.PinNumber*4);
		pGPIOHandle->pGPIOx->CRL |= (pGPIOHandle->PinConfig.PinMode << pGPIOHandle->PinConfig.PinNumber*4);
	}else{
		pGPIOHandle->pGPIOx->CRH &= ~(0x3 << ((pGPIOHandle->PinConfig.PinNumber % 8 )*4));
		pGPIOHandle->pGPIOx->CRH |= (pGPIOHandle->PinConfig.PinMode << ((pGPIOHandle->PinConfig.PinNumber % 8 )*4));
	}
}

static void GPIOSetDirection(GPIO_Handle_t *pGPIOHandle){
	if(pGPIOHandle->PinConfig.PinNumber < 8){
		pGPIOHandle->pGPIOx->CRL &= ~(0x3 << (pGPIOHandle->PinConfig.PinNumber * 4 + 2));
		pGPIOHandle->pGPIOx->CRL |= (pGPIOHandle->PinConfig.PinInOrOut << (pGPIOHandle->PinConfig.PinNumber * 4 + 2));

	}else{
		pGPIOHandle->pGPIOx->CRH &= ~(0x3 << ((pGPIOHandle->PinConfig.PinNumber % 8 ) * 4 + 2));
		pGPIOHandle->pGPIOx->CRH |= (pGPIOHandle->PinConfig.PinInOrOut << ((pGPIOHandle->PinConfig.PinNumber % 8 )* 4 + 2));
	}
}

static void ResetDigits(GPIO_Handle_t *pGPIOHandle, const uint8_t numOfDigitsOnDisplay){
	pGPIOHandle->pGPIOx->BSRR |= ( NUMBER_OF_DIGITS_BITMASK(numOfDigitsOnDisplay) << pGPIOHandle->PinConfig.PinNumber);
}

static void ResetSegments(GPIO_Handle_t *pGPIOHandle){
	pGPIOHandle->pGPIOx->BSRR |= ( ((uint16_t)CLEAN_SEGMENTS_BITMASK) << 0);
}

static void SetSegments(GPIO_Handle_t *pGPIOHandle, const uint16_t *displayedNumber){
	pGPIOHandle->pGPIOx->BRR |= ( (*(displayedNumber+chosenSegmentNum)) << 0);
}

static void SetDigit(GPIO_Handle_t *pGPIOHandle){
	pGPIOHandle->pGPIOx->BRR |= ( (digitsPNPTransistorBitmask[chosenSegmentNum]) << pGPIOHandle->PinConfig.PinNumber);
}


/******************************************************************************************
 ********************** Main functions definitions ****************************************
 ******************************************************************************************/
void GPIO_Init(GPIO_Handle_t *pGPIOHandle){
	RCCEnableGPIOClock(pGPIOHandle);
		GPIOSetMode(pGPIOHandle);
		GPIOSetDirection(pGPIOHandle);
}

void GPIO_WriteToOutput(GPIO_Handle_t *pGPIOHandle, uint8_t pinNumber, uint8_t enOrDis){
	if(enOrDis == ENABLE){
		pGPIOHandle->pGPIOx->BSRR |= (ENABLE << pinNumber);
	}else{
		pGPIOHandle->pGPIOx->BSRR |= (DISABLE << (pinNumber + 16));
	}
}

uint8_t GPIO_ReadFromInput(GPIO_Handle_t *pGPIOHandle, uint8_t PinNumber){
	return pGPIOHandle->pGPIOx->IDR & ( 1 << PinNumber);
}

uint8_t isGPIOOutputSet(GPIO_Handle_t *pGPIOHandle){
	return pGPIOHandle->pGPIOx->ODR & (1 << pGPIOHandle->PinConfig.PinNumber);
}

void SSEG_SetDigit(GPIO_Handle_t *pGPIOHandle, const uint8_t numOfDigitsOnDisplay, const uint16_t *displayedNumber){
	ResetDigits(pGPIOHandle, numOfDigitsOnDisplay);
	ResetSegments(pGPIOHandle);
	SetSegments(pGPIOHandle, displayedNumber);
	SetDigit(pGPIOHandle);

	if(chosenSegmentNum < (numOfDigitsOnDisplay-1)){
		chosenSegmentNum++;
	}else{
		chosenSegmentNum=0;
	}
}

void GPIO_ToggleOutputPin(GPIO_Handle_t *pGPIOHandle, uint8_t pinNumber){
	if(! isGPIOOutputSet(pGPIOHandle) ){
		pGPIOHandle->pGPIOx->BSRR |= (ENABLE << pinNumber);
	}else{
		pGPIOHandle->pGPIOx->BSRR |= (ENABLE << (pinNumber + 16));
	}
}

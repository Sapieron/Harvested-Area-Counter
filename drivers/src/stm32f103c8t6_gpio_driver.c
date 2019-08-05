/*
 * stm32f103c8t6_gpio_driver.c
 *
 *  Created on: 29.07.2019
 *      Author: Pawel
 */

#include <stm32f103c8t6.h>
#include <stm32f103c8t6_gpio_driver.h>
#include <stdint.h>

//mask is needed to set certain outputs high or low
#define numOfDigitsMask(numOfDigits)	((numOfDigits==0)?0:\
										(numOfDigits==1)?1:\
										(numOfDigits==2)?3:\
										(numOfDigits==3)?7:\
										(numOfDigits==4)?15:0)		//maximum 4 digit 7-segment display

/************************************************************
 * ******************** MAIN FUNCTIONS DEFINITIONS***********
 ************************************************************/

#define CLEAN_SEGMENTS 0x6E23


static uint8_t digit[5] = {1, 2, 4, 8, 16};

static uint8_t i=0;

void GPIO_Init(GPIO_Handle_t *pGPIOHandle)		//this function implements only ENABLE of the peripheral
{
	if(pGPIOHandle->pGPIOx == GPIOA)
	{
		RCC->APB2ENR |= (1 << RCC_APB2ENR_IOPAEN);
	}
	else if (pGPIOHandle->pGPIOx == GPIOB)
	{
		RCC->APB2ENR |= (1 << RCC_APB2ENR_IOPBEN);
	}
	else if (pGPIOHandle->pGPIOx == GPIOC)
	{
		RCC->APB2ENR |= (1 << RCC_APB2ENR_IOPCEN);
	}
	else if (pGPIOHandle->pGPIOx == GPIOD)
	{
		RCC->APB2ENR |= (1 << RCC_APB2ENR_IOPDEN);
	}
	else if (pGPIOHandle->pGPIOx == GPIOE)
	{
		RCC->APB2ENR |= (1 << RCC_APB2ENR_IOPEEN);
	}
	if(pGPIOHandle->PinConfig.PinNumber < 8)
	{								//if PinNumer is lesser than 8, then all configs should be made within CRL register
		pGPIOHandle->pGPIOx->CRL &= ~(0x3 << pGPIOHandle->PinConfig.PinNumber*4);
		pGPIOHandle->pGPIOx->CRL &= ~(0x3 << (pGPIOHandle->PinConfig.PinNumber * 4 + 2));
		pGPIOHandle->pGPIOx->CRL |= (pGPIOHandle->PinConfig.PinMode << pGPIOHandle->PinConfig.PinNumber*4);			//Set Mode to output or input
		pGPIOHandle->pGPIOx->CRL |= (pGPIOHandle->PinConfig.PinInOrOut << (pGPIOHandle->PinConfig.PinNumber * 4 + 2));

	}
	else
	{
		pGPIOHandle->pGPIOx->CRH &= ~(0x3 << ((pGPIOHandle->PinConfig.PinNumber % 8 ) * 4 + 2));
		pGPIOHandle->pGPIOx->CRH &= ~(0x3 << ((pGPIOHandle->PinConfig.PinNumber % 8 )*4));
		pGPIOHandle->pGPIOx->CRH |= (pGPIOHandle->PinConfig.PinInOrOut << ((pGPIOHandle->PinConfig.PinNumber % 8 )* 4 + 2));
		pGPIOHandle->pGPIOx->CRH |= (pGPIOHandle->PinConfig.PinMode << ((pGPIOHandle->PinConfig.PinNumber % 8 )*4));

	}
}



void GPIO_WriteToOutput(GPIO_Handle_t *pGPIOHandle, uint8_t pinNumber, uint8_t enOrDis)
{
	if(enOrDis == ENABLE)
	{
		pGPIOHandle->pGPIOx->BSRR |= (ENABLE << pinNumber);		//Atomic set
	}
	else
	{
		pGPIOHandle->pGPIOx->BSRR |= (DISABLE << (pinNumber + 16));
	}
}



uint8_t GPIO_ReadFromInput(GPIO_Handle_t *pGPIOHandle, uint8_t PinNumber)
{
	uint8_t result=0;
	result = (pGPIOHandle->pGPIOx->IDR & ( 1 << PinNumber));

	return result;
}


void SSEG_SetDigit(GPIO_Handle_t *pGPIOHandle, const uint8_t numOfDigits, const uint16_t *displayedNumber)
{
	 //1. Turn off 2 digits
	pGPIOHandle->pGPIOx->BSRR |= ( numOfDigitsMask(numOfDigits) << pGPIOHandle->PinConfig.PinNumber);	//I set 1 in BSRR, which sets digits off


	//2. Reset segments
	pGPIOHandle->pGPIOx->BSRR |= ( ((uint16_t)CLEAN_SEGMENTS) << 0);	//Set bitmask


	//3. Set segments
	pGPIOHandle->pGPIOx->BRR |= ( (*(displayedNumber+i)) << 0);


	//4. Set 1 digit on
	pGPIOHandle->pGPIOx->BRR |= ( (digit[i]) << pGPIOHandle->PinConfig.PinNumber);	//now set 0 to activate transistor of specific digit

	if(i < (numOfDigits-1))		//,,-1" is necessary, as without it one extra cycle is done
				i++;
			else
				i=0;
}



void GPIO_ToggleOutputPin(GPIO_Handle_t *pGPIOHandle, uint8_t pinNumber)
{
	//check if the output is set or reset, then perform write to corresponding BSRR register
	if(!(pGPIOHandle->pGPIOx->ODR & (1 << pinNumber))) 	//check if output is disabled
		{
			pGPIOHandle->pGPIOx->BSRR |= (ENABLE << pinNumber);		//if it's disabled, then enable
		}
		else
		{
			pGPIOHandle->pGPIOx->BSRR |= (ENABLE << (pinNumber + 16));
		}
}

void IRQ_Config(uint32_t IRQ_Number, uint8_t enOrDis)
{
	if(enOrDis == ENABLE)
	{
		if(IRQ_Number <= 31)
			*(NVIC_ISER0) |= (1 << IRQ_Number);
		else if(IRQ_Number > 31 && IRQ_Number <=63)
			*(NVIC_ISER1) |= (1 << IRQ_Number%32);
		else if(IRQ_Number > 63 && IRQ_Number <=80)
			*(NVIC_ISER2) |= (1 << IRQ_Number%32);
	}
	else
	{
		if(IRQ_Number <= 31)
			*(NVIC_ICER0) |= (1 << IRQ_Number);
		else if(IRQ_Number > 31 && IRQ_Number <=63)
			*(NVIC_ICER1) |= (1 << IRQ_Number);
		else if(IRQ_Number > 63 && IRQ_Number <=80)
			*(NVIC_ICER2) |= (1 << IRQ_Number);
	}
}


void GPIO_IRQHandling(EXTI_RegDef_t *pEXTI,uint8_t PinNumber)
{
	//1. Clean the PR register
	if(pEXTI->PR & (ENABLE << PinNumber))
		pEXTI->PR |= (ENABLE << PinNumber);
}

void GPIO_IRQ_EXTI_Init(EXTI_RegDef_t *pEXTI ,uint8_t EXTItriggerType, uint8_t PinNumber)
{
	if(!(RCC->APB2ENR & (ENABLE << 0)))
	{
		RCC->APB2ENR |= (ENABLE << 0);		//set AFIO clock
	}
	pEXTI->IMR |= (ENABLE << PinNumber);
	//set FT or RT
	if(EXTItriggerType == EXTI_IMR_FTSR)
	{
		pEXTI->FTSR |= (ENABLE << PinNumber);
	}
	else if(EXTItriggerType == EXTI_IMR_RTSR)
	{
		pEXTI->RTSR |= (ENABLE << PinNumber);
	}
}





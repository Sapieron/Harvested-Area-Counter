/*
 * stm32f103c8t6_rcc.c
 *
 *  Created on: 02.10.2019
 *      Author: Pawel
 */

#include <stm32f103c8t6.h>
#include <stm32f103c8t6_rcc.h>
#include <stm32f103c8t6_gpio.h>
#include <stdint.h>

/******************************************************************************************
 ********************** Private functions *************************************************
 ******************************************************************************************/
static void EnableGPIOClock(uint8_t RCC_APB2ENR_IOPxEN){
	RCC->APB2ENR |= (ENABLE << RCC_APB2ENR_IOPxEN);
}


/******************************************************************************************
 ********************** Main functions definitions ****************************************
 ******************************************************************************************/
void RCCEnableGPIOClock(GPIO_Handle_t *pGPIOHandle){
	if(pGPIOHandle->pGPIOx == GPIOA){
		EnableGPIOClock(RCC_APB2ENR_IOPAEN);
	}else if (pGPIOHandle->pGPIOx == GPIOB){
		EnableGPIOClock(RCC_APB2ENR_IOPBEN);
	}else if (pGPIOHandle->pGPIOx == GPIOC){
		EnableGPIOClock(RCC_APB2ENR_IOPCEN);
	}else if (pGPIOHandle->pGPIOx == GPIOD){
		EnableGPIOClock(RCC_APB2ENR_IOPDEN);
	}else if (pGPIOHandle->pGPIOx == GPIOE){
		EnableGPIOClock(RCC_APB2ENR_IOPEEN);
	}
}

void RCC_ResetTimerClock(uint8_t RCC_APB1RSTR_Timer){
	RCC->APB1RSTR |= (ENABLE << RCC_APB1RSTR_Timer);
	RCC->APB1RSTR &=~(ENABLE << RCC_APB1RSTR_Timer);
}

void RCC_SetTimerClock(uint8_t RCC_APB1RSTR_Timer){
	RCC->APB1ENR |= (ENABLE << RCC_APB1RSTR_Timer);
}

void RCC_SetAFIOClockOn(){
	RCC->APB2ENR |= (ENABLE << RCC_APB2ENR_AFIOEN);
}

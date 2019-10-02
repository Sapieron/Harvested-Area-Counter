/*
 * stm32f103c8t6_interrupt.c
 *
 *  Created on: 02.10.2019
 *      Author: Pawel
 */

#include <stm32f103c8t6.h>
#include <stm32f103c8t6_rcc.h>
#include <stm32f103c8t6_interrupt.h>
#include <stdint.h>

/******************************************************************************************
 ********************** Private functions *************************************************
 ******************************************************************************************/
static void ResetPendingRegister(EXTI_RegDef_t *pEXTI,uint8_t pinNumber){
	pEXTI->PR |= (ENABLE << pinNumber);
}

static void InterruptMaskingHandling(EXTI_RegDef_t *pEXTI, uint8_t enOrDis, uint8_t pinNumber){
	if(enOrDis == DISABLE)
		pEXTI->IMR |= (ENABLE << pinNumber);
	else
		pEXTI->IMR &= ~(ENABLE << pinNumber);
}

static uint8_t isAFIOClockOn(){
	return (RCC->APB2ENR & (ENABLE << RCC_APB2ENR_AFIOEN));
}

static void SetTriggerType(EXTI_RegDef_t *pEXTI ,uint8_t EXTItriggerType, uint8_t pinNumber){
	if(EXTItriggerType == EXTI_IMR_FTSR)
		pEXTI->FTSR |= (ENABLE << pinNumber);
	else if(EXTItriggerType == EXTI_IMR_RTSR)
		pEXTI->RTSR |= (ENABLE << pinNumber);
}

/******************************************************************************************
 ********************** Main functions definitions ****************************************
 ******************************************************************************************/
void IRQ_PendingPinInterrupt(EXTI_RegDef_t *pEXTI,uint8_t pinNumber){
	if(pEXTI->PR & (ENABLE << pinNumber))
		ResetPendingRegister(pEXTI, pinNumber);
}

void GPIO_IRQ_EXTI_Init(EXTI_RegDef_t *pEXTI ,uint8_t EXTItriggerType, uint8_t pinNumber){
	if(!isAFIOClockOn())
		RCC_SetAFIOClockOn();

	InterruptMaskingHandling(pEXTI, DISABLE, pinNumber);
	SetTriggerType(pEXTI, EXTItriggerType, pinNumber);
}

void IRQ_IRQNumberHandling(uint32_t IRQ_Number, uint8_t enOrDis){
	if(enOrDis == ENABLE){
		if(IRQ_Number <= 31)
			*(NVIC_ISER0) |= (ENABLE << IRQ_Number);
		else if(IRQ_Number > 31 && IRQ_Number <=63)
			*(NVIC_ISER1) |= (ENABLE << IRQ_Number%32);
		else if(IRQ_Number > 63 && IRQ_Number <=80)
			*(NVIC_ISER2) |= (ENABLE << IRQ_Number%32);
	}else{
		if(IRQ_Number <= 31)
			*(NVIC_ICER0) |= (ENABLE << IRQ_Number);
		else if(IRQ_Number > 31 && IRQ_Number <=63)
			*(NVIC_ICER1) |= (ENABLE << IRQ_Number);
		else if(IRQ_Number > 63 && IRQ_Number <=80)
			*(NVIC_ICER2) |= (ENABLE << IRQ_Number);
	}
}

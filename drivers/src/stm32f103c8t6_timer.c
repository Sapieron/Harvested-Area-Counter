/*
 * stm32f103c8t6_timer_driver.c
 *
 *  Created on: 29.07.2019
 *      Author: Pawel
 */

#include <stdint.h>
#include <string.h>
#include <stm32f103c8t6.h>
#include <stm32f103c8t6_rcc.h>
#include <stm32f103c8t6_timer.h>

/******************************************************************************************
 ********************** Private functions *************************************************
 ******************************************************************************************/
static void TIMER6_7SetPrescalerRegister(TIMER6_7_Handle_t *timer){
	timer->pTIMERreg->PSC = (timer->Config.prescalerVal << TIMER6_7_PSC_PSC);
}

static void TIMER6_7SetAutoReloadRegister(TIMER6_7_Handle_t *timer){
	timer->pTIMERreg->ARR = (timer->Config.autoReloadVal << TIMER6_7_ARR_ARR);
}

static void TIMER6_7SetEventGenerationRegister(TIMER6_7_Handle_t *timer){
	timer->pTIMERreg->EGR |= (timer->Config.initialize << TIMER6_7_EGR_UG);
}

static void TIMER6_7SetInterruptEnableRegister(TIMER6_7_Handle_t *timer){
	timer->pTIMERreg->DIER |= (timer->Config.interruptEnable << TIMER6_7_DIER_UIE);
}

static void TIMER2_5SetPrescalerRegister(TIMER2_5_Handle_t *timer){
	timer->pTIMERreg->PSC = (timer->Config.prescalerVal << TIMER2_5_PSC_PSC);
}

static void TIMER2_5SetAutoReloadRegister(TIMER2_5_Handle_t *timer){
	timer->pTIMERreg->ARR = (timer->Config.autoReloadVal << TIMER2_5_ARR_ARR);
}

static void TIMER2_5SetEventGenerationRegister(TIMER2_5_Handle_t *timer){
	timer->pTIMERreg->EGR |= (timer->Config.initialize << TIMER2_5_EGR_UG);
}

static void TIMER2_5SetInterruptEnableRegister(TIMER2_5_Handle_t *timer){
	timer->pTIMERreg->DIER |= (timer->Config.interruptEnable << TIMER2_5_DIER_UIE);
}

static void Timer2_5CounterEnable(TIMER2_5_Handle_t *pTimerHandle){
	pTimerHandle->pTIMERreg->CR1 |= (ENABLE << TIMER2_5_CR1_CEN);
}

static void Timer6_7CounterDisable(TIMER6_7_Handle_t *pTimerHandle){
	pTimerHandle->pTIMERreg->CR1 &= ~(ENABLE << TIMER6_7_CR1_CEN);
}

static void Timer6_7CounterEnable(TIMER6_7_Handle_t *pTimerHandle){
	pTimerHandle->pTIMERreg->CR1 |= (ENABLE << TIMER6_7_CR1_CEN);
}


/******************************************************************************************
 ********************** Main functions definitions ****************************************
 ******************************************************************************************/
void TIM6_7_Init(TIMER6_7_Handle_t *timer, uint8_t clockEnOrDis){
	Timer6_7CounterDisable(timer);

	if(timer->pTIMERreg == (TIMER6_7_RegDef_t*)TIM6_BASE_ADDR){
		RCC_ResetTimerClock(RCC_APB1RSTR_TIMER6);
		RCC_SetTimerClock(RCC_APB1ENR_TIMER6);
	}else{
		RCC_ResetTimerClock(RCC_APB1RSTR_TIMER7);
		RCC_SetTimerClock(RCC_APB1ENR_TIMER7);
	}
	TIMER6_7SetPrescalerRegister(timer);
	TIMER6_7SetAutoReloadRegister(timer);
	TIMER6_7SetEventGenerationRegister(timer);
	TIMER6_7SetInterruptEnableRegister(timer);

	if(clockEnOrDis == ENABLE)
		Timer6_7CounterEnable(timer);
	else if(clockEnOrDis == DISABLE)
		Timer6_7CounterDisable(timer);
}

void TIM2_5_Init(TIMER2_5_Handle_t *timer, uint8_t clockEnOrDis){
	Timer2_5CounterDisable(timer);

	if(timer->pTIMERreg == (TIMER2_5_RegDef_t*)TIM2_BASE_ADDR){
		RCC_ResetTimerClock(RCC_APB1RSTR_TIMER2);
		RCC_SetTimerClock(RCC_APB1ENR_TIMER2);
	} else if(timer->pTIMERreg == (TIMER2_5_RegDef_t*)TIM3_BASE_ADDR){
		RCC_ResetTimerClock(RCC_APB1RSTR_TIMER3);
		RCC_SetTimerClock(RCC_APB1ENR_TIMER3);
	}

	TIMER2_5SetPrescalerRegister(timer);
	TIMER2_5SetAutoReloadRegister(timer);
	TIMER2_5SetEventGenerationRegister(timer);
	TIMER2_5SetInterruptEnableRegister(timer);

	if(clockEnOrDis == ENABLE)
		Timer2_5CounterEnable(timer);
	else if(clockEnOrDis == DISABLE)
		Timer2_5CounterDisable(timer);
}

void SensorHandling(TIMER2_5_Handle_t *pTimerHandle){
	pTimerHandle->pTIMERreg->CNT = 0;
	Timer2_5CounterEnable(pTimerHandle);
}

uint8_t isTimerFlagSet(TIMER2_5_Handle_t *pTimerHandle){
	return pTimerHandle->pTIMERreg->SR & (1 << TIMER2_5_SR_UIF);
}

void ClearFlagStatus(TIMER2_5_Handle_t *pTimerHandle){
	pTimerHandle->pTIMERreg->SR &= ~(1 << TIMER2_5_SR_UIF);
}

void Timer2_5CounterDisable(TIMER2_5_Handle_t *pTimerHandle){
	pTimerHandle->pTIMERreg->CR1 &= ~(ENABLE << TIMER2_5_CR1_CEN);
}

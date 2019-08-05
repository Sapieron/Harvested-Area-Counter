/*
 * stm32f103c8t6_timer_driver.c
 *
 *  Created on: 29.07.2019
 *      Author: Pawel
 */

#include <stm32f103c8t6_timer_driver.h>
#include <stm32f103c8t6.h>
#include <stdint.h>
#include <string.h>

//TIM6 global interrupt 0x00000118



void TIM6_7_Init(TIMER6_7_Handle_t *timer, uint8_t clockEnOrDis)
{
	timer->pTIMERreg->CR1 &=~ (1 << TIMER6_7_CR1_CEN);	//make sure counter is off

	if(timer->pTIMERreg == (TIMER6_7_RegDef_t*)TIM6_BASE_ADDR){
		RCC->APB1RSTR |= (ENABLE << 0);
		RCC->APB1RSTR &=~(ENABLE << 0);
		RCC->APB1ENR |= (ENABLE << 0);		//Clock never sets - I haven't solved this problem yet
	}
	else
	{
		RCC->APB1RSTR |= (ENABLE << RCC_APB1RSTR_TIMER7);
		RCC->APB1RSTR &=~(ENABLE << RCC_APB1RSTR_TIMER7);
		RCC->APB1ENR |= (ENABLE << RCC_APB1ENR_TIMER7);
	}
	timer->pTIMERreg->PSC |= (timer->Config.prescalerVal << TIMER6_7_PSC_PSC);	//set prescaler value
	timer->pTIMERreg->ARR |= (timer->Config.autoReloadVal << TIMER6_7_ARR_ARR);	//set autoreload value

	timer->pTIMERreg->EGR |= (timer->Config.initialize << TIMER6_7_EGR_UG);
	timer->pTIMERreg->DIER |= (timer->Config.interruptEnable << TIMER6_7_DIER_UIE);

	if(clockEnOrDis == ENABLE)
	timer->pTIMERreg->CR1 |= (ENABLE << TIMER6_7_CR1_CEN);	//enable counter
	else if(clockEnOrDis == DISABLE)
		timer->pTIMERreg->CR1 &= ~(ENABLE << TIMER6_7_CR1_CEN);	//enable counter
}


void TIM2_5_Init(TIMER2_5_Handle_t *timer, uint8_t clockEnOrDis)
{
	timer->pTIMERreg->CR1 &=~ (ENABLE << TIMER2_5_CR1_CEN);	//make sure counter is off

	if(timer->pTIMERreg == (TIMER2_5_RegDef_t*)TIM2_BASE_ADDR){
		RCC->APB1RSTR |= (ENABLE << RCC_APB1RSTR_TIMER2);
		RCC->APB1RSTR &=~(ENABLE << RCC_APB1RSTR_TIMER2);
		RCC->APB1ENR |= (ENABLE << RCC_APB1ENR_TIMER2);
	} else if(timer->pTIMERreg == (TIMER2_5_RegDef_t*)TIM3_BASE_ADDR){
		RCC->APB1RSTR |= (ENABLE << RCC_APB1RSTR_TIMER3);
		RCC->APB1RSTR &=~(ENABLE << RCC_APB1RSTR_TIMER3);
		RCC->APB1ENR |= (ENABLE << RCC_APB1ENR_TIMER3);
	}
	timer->pTIMERreg->PSC |= (timer->Config.prescalerVal << TIMER2_5_PSC_PSC);	//set prescaler value

	//timer->pTIMERreg->CR1 |= (1 << TIMER2_5_CR1_ARPE);
	timer->pTIMERreg->ARR = (timer->Config.autoReloadVal << TIMER2_5_ARR_ARR);	//set autoreload value

	timer->pTIMERreg->EGR |= (timer->Config.initialize << TIMER2_5_EGR_UG);
	timer->pTIMERreg->DIER |= (timer->Config.interruptEnable << TIMER2_5_DIER_UIE);

	if(clockEnOrDis == ENABLE)
		timer->pTIMERreg->CR1 |= (ENABLE << TIMER2_5_CR1_CEN);	//enable counter
	else if(clockEnOrDis == DISABLE)
		timer->pTIMERreg->CR1 &= ~(ENABLE << TIMER2_5_CR1_CEN);	//enable counter
}

void TimerHandling(TIMER2_5_Handle_t *pTimerHandle)
{
	pTimerHandle->pTIMERreg->CR1 &= ~(ENABLE << TIMER2_5_CR1_CEN);
}

void SensorHandling(TIMER2_5_Handle_t *pTimerHandle)
{
	pTimerHandle->pTIMERreg->CNT = 0;
	pTimerHandle->pTIMERreg->CR1 |= (ENABLE << TIMER2_5_CR1_CEN);
}


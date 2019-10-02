/*
 * stm32f103c8t6_timer_driver.h
 *
 *  Created on: 29.07.2019
 *      Author: Pawel
 */

#ifndef INC_STM32F103C8T6_TIMER_H_
#define INC_STM32F103C8T6_TIMER_H_

#include <stm32f103c8t6.h>

/******************************************************************************************
 ********************************* Typedef structures *************************************
 ******************************************************************************************/
typedef struct{
	uint16_t prescalerVal;
	uint16_t autoReloadVal;
	uint16_t interruptEnable;
	uint16_t initialize;
}TIMER_Config_t;

typedef struct{
	TIMER6_7_RegDef_t *pTIMERreg;
	TIMER_Config_t	Config;
}TIMER6_7_Handle_t;

typedef struct{
	TIMER2_5_RegDef_t *pTIMERreg;
	TIMER_Config_t	Config;
}TIMER2_5_Handle_t;


/******************************************************************************************
 ********************** Main functions definitions ****************************************
 ******************************************************************************************/

void TIM6_7_Init(TIMER6_7_Handle_t *timer, uint8_t clockEnOrDis);
void TIM2_5_Init(TIMER2_5_Handle_t *timer, uint8_t clockEnOrDis);

void IRQ_IRQNumberHandling(uint32_t IRQ_Number, uint8_t enOrDis);

void Timer2_5CounterDisable(TIMER2_5_Handle_t *pTimerHandle);

void SensorHandling(TIMER2_5_Handle_t *pTimerHandle);

uint8_t isTimerFlagSet(TIMER2_5_Handle_t *pTimerHandle);

void ClearFlagStatus(TIMER2_5_Handle_t *pTimerHandle);


/******************************************************************************************
 ********************************* Registers OF TIMERS 2 to 5 *****************************
 ******************************************************************************************/

#define TIMER2_5_CR1_CEN		0	//Counter enable
#define TIMER2_5_CR1_UDIS		1	//Update disable
#define TIMER2_5_CR1_URS		2	//Update request source
#define TIMER2_5_CR1_OPM		3	//One-pulse mode
#define TIMER2_5_CR1_ARPE		7	//Auto-reload preload enable

#define TIMER2_5_CR2_MMS		4	//Master Mode Selection

#define TIMER2_5_DIER_UIE		0	//Update interrupt enable
#define TIMER2_5_DIER_TIE		6	//Trigger interrupt enable

#define TIMER2_5_SR_UIF			0	//Update interrupt flag
#define TIMER2_5_SR_TIF			6	//trigger interrupt flag


#define TIMER2_5_EGR_UG			0	//Update generation
#define TIMER2_5_EGR_TG			6	//Trigger generation

#define TIMER2_5_CNT_CNT		0	//Counter value

#define TIMER2_5_PSC_PSC		0	//Prescaler value

#define TIMER2_5_ARR_ARR		0	//Auto-reload value


/******************************************************************************************
 ********************************* Registers OF TIMERS 6 to 7 *****************************
 ******************************************************************************************/

#define TIMER6_7_CR1_CEN		0	//Counter enable
#define TIMER6_7_CR1_UDIS		1	//Update disable
#define TIMER6_7_CR1_URS		2	//Update request source
#define TIMER6_7_CR1_OPM		3	//One-pulse mode
#define TIMER6_7_CR1_ARPE		7	//Auto-reload preload enable

#define TIMER6_7_CR2_MMS		4	//Master Mode Selection

#define TIMER6_7_DIER_UIE		0	//Update interrupt enable
#define TIMER6_7_DIER_UDE		8	//Update DMA request enable

#define TIMER6_7_SR_UIF			0	//Update interrupt flag
#define TIMER6_7_SR_TIF			6	//T
#define TIMER6_7_EGR_UG			0	//Update generation

#define TIMER6_7_CNT_CNT		0	//Counter value

#define TIMER6_7_PSC_PSC		0	//Prescaler value

#define TIMER6_7_ARR_ARR		0	//Auto-reload value


#endif /* INC_STM32F103C8T6_TIMER_H_ */

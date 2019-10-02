/*
 * stm32f103c8t6_rcc.h
 *
 *  Created on: 30.09.2019
 *      Author: Pawel
 */

#ifndef INC_STM32F103C8T6_RCC_H_
#define INC_STM32F103C8T6_RCC_H_

#include <stm32f103c8t6.h>

//TODO dodac funkcje rcc tutaj


/******************************************************************
 * **************** RCC Functions *********************************
 ******************************************************************/

#define RCC_APB2ENR_IOPAEN		2
#define RCC_APB2ENR_IOPBEN		3
#define RCC_APB2ENR_IOPCEN		4
#define RCC_APB2ENR_IOPDEN		5
#define RCC_APB2ENR_IOPEEN		6

#define RCC_APB1ENR_TIMER2		0
#define RCC_APB1ENR_TIMER3		1
#define RCC_APB1ENR_TIMER6		4
#define RCC_APB1ENR_TIMER7		5

#define RCC_APB1RSTR_TIMER2		0
#define RCC_APB1RSTR_TIMER3		1
#define RCC_APB1RSTR_TIMER6		4
#define RCC_APB1RSTR_TIMER7		5

#endif /* INC_STM32F103C8T6_RCC_H_ */

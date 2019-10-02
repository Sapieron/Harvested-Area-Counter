/*
 * stm32f103c8t6_interrupt.h
 *
 *  Created on: 30.09.2019
 *      Author: Pawel
 */

#ifndef INC_STM32F103C8T6_INTERRUPT_H_
#define INC_STM32F103C8T6_INTERRUPT_H_

#include <stdint.h>
#include <stm32f103c8t6.h>

//TODO: dodac tutaj funkcje irq

/***************************************************
 * ******************* IRQ numbers******************
 ***************************************************/


#define IRQ_TIMER2				28
#define IRQ_TIMER3				29
#define IRQ_TIMER6				54
#define IRQ_TIMER7				55
#define IRQ_EXTI0				6
#define IRQ_EXTI1				7
#define IRQ_EXTI2				8
#define IRQ_EXTI3				9
#define IRQ_EXTI4				10
#define IRQ_EXTI9_5				23
#define IRQ_EXTI15_10			40



/***************************************************
 * ******************* IRQ numbers******************
 ***************************************************/

#define EXTI_IMR_FTSR				0
#define EXTI_IMR_RTSR				1


#endif /* INC_STM32F103C8T6_INTERRUPT_H_ */

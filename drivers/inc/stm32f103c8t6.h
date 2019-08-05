/*
 * stm32f103c8t6.h
 *
 *  Created on: 29.07.2019
 *      Author: Pawel
 */

#ifndef INC_STM32F103C8T6_H_
#define INC_STM32F103C8T6_H_

#include <stdint.h>

/*****************************************************************
 * ******************** BASE ADDRESSES DEFINES********************
 *****************************************************************/

/*
 * Busses addresses
 */
#define APB1_BASE_ADDR			0x40000000
#define APB2_BASE_ADDR			0x40010000
#define AHB_BASE_ADDR			0x40018000
#define SRAM_BASE_ADDR			0x20000000
#define FLASH_BASE_ADDR			0x08000000



/*
 * RCC Base Address
 */
#define RCC_BASE_ADDR			(AHB_BASE_ADDR + 0x9000)



/*
 * GPIO Base Addresses
 */
#define GPIOA_BASE_ADDR			(APB2_BASE_ADDR + 0x800)
#define GPIOB_BASE_ADDR			(APB2_BASE_ADDR + 0xC00)
#define GPIOC_BASE_ADDR			(APB2_BASE_ADDR + 0x1000)
#define GPIOD_BASE_ADDR			(APB2_BASE_ADDR + 0x1400)
#define GPIOE_BASE_ADDR			(APB2_BASE_ADDR + 0x1800)



/*
 * EXTI Base Addresses
 */
#define EXTI_BASE_ADDR			(APB2_BASE_ADDR + 0x400)



/*
 * NVIC Base addresses
 */

#define NVIC_ISER0_BASEADDR		0xE000E100						//DOCUMENTATION PM0056
#define NVIC_ISER1_BASEADDR		(NVIC_ISER0_BASEADDR + 0x04)
#define NVIC_ISER2_BASEADDR		(NVIC_ISER0_BASEADDR + 0x08)

#define NVIC_ICER0_BASEADDR		(NVIC_ISER0_BASEADDR + 0x80)
#define NVIC_ICER1_BASEADDR		(NVIC_ISER0_BASEADDR + 0x84)
#define NVIC_ICER2_BASEADDR		(NVIC_ISER0_BASEADDR + 0x88)

/*
 * TIMER Base Addresses
 */
#define TIM2_BASE_ADDR			(APB1_BASE_ADDR)
#define TIM3_BASE_ADDR			(APB1_BASE_ADDR + 0x0400)
#define TIM6_BASE_ADDR			(APB1_BASE_ADDR + 0x1000)	//BASIC TIMER
#define TIM7_BASE_ADDR			(APB1_BASE_ADDR + 0x1400)	//BASIC TIMER







/******************************************************************
 * **************** HANDLE STRUCTURES DEFINES *********************
 ******************************************************************/

typedef struct
{
	volatile uint32_t CR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t APB2RSTR;
	volatile uint32_t APB1RSTR;
	volatile uint32_t AHBENR;
	volatile uint32_t APB2ENR;
	volatile uint32_t APB1ENR;
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
	volatile uint32_t AHBSTR;
	volatile uint32_t CFGR2;
}RCC_RegDef_t;


typedef struct
{
	volatile uint32_t CRL;
	volatile uint32_t CRH;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t BRR;
	volatile uint32_t LCKR;
}GPIO_RegDef_t;

typedef struct
{
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t reserved;
	volatile uint32_t DIER;
	volatile uint32_t SR;
	volatile uint32_t EGR;
	volatile uint32_t reserved2[3];
	volatile uint32_t CNT;
	volatile uint32_t PSC;
	volatile uint32_t ARR;		//AUTORELOAD REGISTER
}TIMER6_7_RegDef_t;

typedef struct
{
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t SMCR;
	volatile uint32_t DIER;
	volatile uint32_t SR;
	volatile uint32_t EGR;
	volatile uint32_t CCMR1;
	volatile uint32_t CCMR2;
	volatile uint32_t CCER;
	volatile uint32_t CNT;
	volatile uint32_t PSC;
	volatile uint32_t ARR;		//AUTORELOAD REGISTER
	volatile uint32_t reserved1;
	volatile uint32_t CCR1;
	volatile uint32_t CCR2;
	volatile uint32_t CCR3;
	volatile uint32_t CCR4;
	volatile uint32_t reserved2;
	volatile uint32_t DCR;
	volatile uint32_t DMAR;
}TIMER2_5_RegDef_t;

typedef struct
{
	volatile uint32_t IMR;
	volatile uint32_t EMR;
	volatile uint32_t RTSR;
	volatile uint32_t FTSR;
	volatile uint32_t SWIER;
	volatile uint32_t PR;
}EXTI_RegDef_t;

/******************************************************************
 * **************** GPIO Macros ************************************
 ******************************************************************/

#define GPIOA					((GPIO_RegDef_t*)GPIOA_BASE_ADDR)
#define GPIOB					((GPIO_RegDef_t*)GPIOB_BASE_ADDR)
#define GPIOC					((GPIO_RegDef_t*)GPIOC_BASE_ADDR)
#define GPIOD					((GPIO_RegDef_t*)GPIOD_BASE_ADDR)
#define GPIOE					((GPIO_RegDef_t*)GPIOE_BASE_ADDR)





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

#define RCC_APB2ENR_EXTI

#define RCC 					((RCC_RegDef_t*)RCC_BASE_ADDR)

#define TIMER2					((TIMER2_5_RegDef_t*)TIM2_BASE_ADDR)
#define TIMER3					((TIMER2_5_RegDef_t*)TIM3_BASE_ADDR)
#define TIMER6					((TIMER6_7_RegDef_t*)TIM6_BASE_ADDR)
#define TIMER7					((TIMER6_7_RegDef_t*)TIM7_BASE_ADDR)





/****************************************************************
 * ****************** NVIC and EXTI pointers *****************************
 ****************************************************************/

#define NVIC_ISER0				((volatile uint32_t*)NVIC_ISER0_BASEADDR)
#define NVIC_ISER1				((volatile uint32_t*)NVIC_ISER1_BASEADDR)
#define NVIC_ISER2				((volatile uint32_t*)NVIC_ISER2_BASEADDR)

#define NVIC_ICER0				((volatile uint32_t*)NVIC_ICER0_BASEADDR)
#define NVIC_ICER1				((volatile uint32_t*)NVIC_ICER1_BASEADDR)
#define NVIC_ICER2				((volatile uint32_t*)NVIC_ICER2_BASEADDR)

#define EXTI_POINTER			((EXTI_RegDef_t*)EXTI_BASE_ADDR)



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


/*****************************************************
 * *********** Generic macros*************************
 *****************************************************/

#define F_CPU					8000000		//CPU frequency is 8MHz
#define ENABLE					1
#define DISABLE					0
#define SET_PNP					0
#define RESET_PNP				1

#endif /* INC_STM32F103C8T6_H_ */

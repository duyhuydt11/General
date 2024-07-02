/************************************************************************************************************************************************/
																										//INCLUDE
/***********************************************************************************************************************************************/

#ifndef __THUVIEN_1_H
#define __THUVIEN_1_H

#include <stdio.h>
#include <stdint.h>
#include "stm32f10x.h"
#include <math.h>

/************************************************************************************************************************************************/
																										//DEFINE
/***********************************************************************************************************************************************/


#define __IO   								volatile   

#define RCC_ADDRESS 					0x40021000
#define GPIO_PORTA_ADDRESS 		0x40010800
#define GPIO_PORTB_ADDRESS 		0x40010C00
#define GPIO_PORTC_ADDRESS 		0x40011000
#define GPIO_PORTD_ADDRESS 		0x40011400
#define GPIO_PORTE_ADDRESS 		0x40011800
#define GPIO_PORTF_ADDRESS 		0x40011C00
#define GPIO_PORTG_ADDRESS 		0x40012000

// Add some define AFIO
#define RCC_APB2Periph_AFIOA   ((uint32_t)0x00000005)
#define RCC_APB2Periph_AFIOB   ((uint32_t)0x00000009)
#define RCC_APB2Periph_AFIOC   ((uint32_t)0x000000011)
#define RCC_APB2Periph_AFIOD   ((uint32_t)0x000000021)
#define RCC_APB2Periph_AFIOE   ((uint32_t)0x000000041)
#define RCC_APB2Periph_AFIOF   ((uint32_t)0x000000081)
#define RCC_APB2Periph_AFIOG   ((uint32_t)0x0000000101)

// Register address
#define My_GPIOC               	((My_GPIO_REG_TYPE *) GPIOC_BASE)
#define My_RCC                 	((My_RRC_REG_TYPE *) RCC_BASE)
#define My_GPIOB               	((My_GPIO_REG_TYPE *) GPIOB_BASE)
#define My_GPIOA               	((My_GPIO_REG_TYPE *) GPIOA_BASE)
#define My_AFIO               	((My_AFIO_TypeDef *)AFIO_BASE)
#define My_EXTI                	((My_EXTI_TypeDef *)EXTI_BASE)
/************************************************************************************************************************************************/
																										//CONFIG REGISTER MODE
/***********************************************************************************************************************************************/

//struct RCC register
typedef struct
{
	__IO uint32_t My_RCC_CR;
	__IO uint32_t My_RCC_CFGR;
	__IO uint32_t My_RCC_CIR;
	__IO uint32_t My_RCC_APB2RSTR;
	__IO uint32_t My_RCC_APB1RSTR;
	__IO uint32_t My_RCC_AHBENR;
	__IO uint32_t My_RCC_APB2ENR;
	__IO uint32_t My_RCC_APB1ENR;
	__IO uint32_t My_RCC_BDCR;
	__IO uint32_t My_RCC_CSR;
	__IO uint32_t My_RCC_AHBRSTR;
	__IO uint32_t My_RCC_CFGR2;

}My_RRC_REG_TYPE;

// struct for GPIO register
typedef struct
{
	__IO uint32_t My_CRL;
	__IO uint32_t My_CRH;
	__IO uint32_t My_IDR;
	__IO uint32_t My_ODR;
	__IO uint32_t My_BSRR;
	__IO uint32_t My_BRR;
	__IO uint32_t My_LCKR;

}My_GPIO_REG_TYPE;

// struct for AFIO register
typedef struct
{
  __IO uint32_t My_EVCR;
  __IO uint32_t My_MAPR;
  __IO uint32_t My_EXTICR[4];
  uint32_t My_RESERVED0;
  __IO uint32_t MAPR2;  
} My_AFIO_TypeDef;

// External Interrupt register
typedef struct
{
  __IO uint32_t My_IMR;
  __IO uint32_t My_EMR;
  __IO uint32_t My_RTSR;
  __IO uint32_t My_FTSR;
  __IO uint32_t My_SWIER;
  __IO uint32_t My_PR;
} My_EXTI_TypeDef;


// config for speed gpio
typedef enum
{ My_GPIO_Input_Mode = 0,
	My_GPIO_Speed_10MHz = 1,
  My_GPIO_Speed_2MHz, 
  My_GPIO_Speed_50MHz
}My_GPIOSpeed_TypeDef;

// config for mode gpio
typedef enum
{ My_GPIO_Mode_AIN = 0x0,
  My_GPIO_Mode_IN_FLOATING = 0x04,
  My_GPIO_Mode_Input = 0x08,
	My_GPIO_Mode_Input_IT = 0x09,
  My_GPIO_Mode_Out_OD = 0x14,
  My_GPIO_Mode_Out_PP = 0x10,
  My_GPIO_Mode_AF_OD = 0x1C,
  My_GPIO_Mode_AF_PP = 0x18
}My_GPIOMode_TypeDef;

typedef enum
{ My_IT_Mode_It_Input_Rising = 0x09,
	My_IT_Mode_It_Input_Falling = 0x0A,
	My_IT_Mode_It_Input_Rising_Falling = 0x0B
}My_ExItr_Mode_TypeDef;



// config for pull mode gpio
typedef enum
{
	My_NoPull = 0u,
  My_PD = 1u,
  My_PU = 2u
} My_Pull_Mode;

// config for the oder of gpio in line
typedef enum
{
	My_AFIO_Port_A = 0u,
  My_AFIO_Port_B = 1u,
  My_AFIO_Port_C,
	My_AFIO_Port_D,
	My_AFIO_Port_E,
	My_AFIO_Port_F,
	My_AFIO_Port_G,
	My_AFIO_No_Port = 7u
} My_AFIO_Exitr_Port;

// full config for gpio mode of pin
typedef struct
{	
	uint16_t My_GPIO_Pin;     
  My_GPIOSpeed_TypeDef My_GPIO_Speed;                                
  My_GPIOMode_TypeDef My_GPIO_Mode;   
	My_Pull_Mode My_GPIO_Pull;	
	My_AFIO_Exitr_Port My_AFIO_Pin_Mode;
}My_GPIO_InitTypeDef;

// config for basic gpio bit
typedef enum
{
  GPIO_Pin_Reset = 0u,
  GPIO_Pin_Set
} My_GPIO_PinState;


/************************************************************************************************************************************************/
																										//FUNCTION PROTOTYPE
/***********************************************************************************************************************************************/

void GPIO_Config(void);
void Delay(__IO uint32_t ms);
void GPIO_Toggle_Pin(My_GPIO_REG_TYPE* GPIOx, uint32_t GPIO_Pin);
void ClockSource(uint32_t RCC_Port,My_GPIO_REG_TYPE* GPIOx ,My_GPIO_InitTypeDef* GPIO_InitStruct);
void GPIO_Reset_Pin(My_GPIO_REG_TYPE* GPIOx, uint32_t GPIO_Pin);
void GPIO_Set_Pin(My_GPIO_REG_TYPE* GPIOx, uint32_t GPIO_Pin);
uint32_t GPIO_Read_Pin(My_GPIO_REG_TYPE* GPIOx, uint32_t GPIO_Pin);
uint32_t Count_Debounce_Button(My_GPIO_REG_TYPE* GPIOx, uint32_t GPIO_Pin, My_Pull_Mode Mode_Pull);
uint32_t GPIO_Button_Pin(My_GPIO_REG_TYPE* GPIOx, uint32_t GPIO_Pin, My_Pull_Mode Mode_Pull);
void External_Interupt_Setup_Config(My_AFIO_Exitr_Port AFIO_Port, uint32_t GPIO_Pin, My_ExItr_Mode_TypeDef Mode);
void External_Interupt_Setup_Priority(uint16_t PreemptPriority, uint16_t SubPriority, IRQn_Type IRQn);
uint8_t Pin_Numerical_Order(uint32_t Pin);
uint32_t GPIO_Read_Interupt_Line(uint32_t Line);
void GPIO_Erase_Interupt_Line(uint32_t Line);
#endif

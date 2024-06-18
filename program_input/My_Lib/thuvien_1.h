//**************************************************INCLUDE***********************************************************
#include <stdio.h>
#include <stdint.h>
#include "stm32f10x.h"

//**************************************************DEFINE************************************************************

#define __IO   								volatile   

#define RCC_ADDRESS 					0x40021000
#define GPIO_PORTA_ADDRESS 		0x40010800
#define GPIO_PORTB_ADDRESS 		0x40010C00
#define GPIO_PORTC_ADDRESS 		0x40011000
#define GPIO_PORTD_ADDRESS 		0x40011400
#define GPIO_PORTE_ADDRESS 		0x40011800
#define GPIO_PORTF_ADDRESS 		0x40011C00
#define GPIO_PORTG_ADDRESS 		0x40012000
#define My_GPIOC               ((My_GPIO_REG_TYPE *) GPIOC_BASE)
#define My_RCC                 ((My_RRC_REG_TYPE *) RCC_BASE)
#define My_GPIOB               ((My_GPIO_REG_TYPE *) GPIOB_BASE)


//**************************************************GLOBAL VARIABLE***************************************************************

//struct MY_RRC_REG_TYPE
// struct MY_RRC_REG_TYPE 1,2,3;
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

typedef enum
{ My_GPIO_Input_Mode = 0,
	My_GPIO_Speed_10MHz = 1,
  My_GPIO_Speed_2MHz, 
  My_GPIO_Speed_50MHz
}My_GPIOSpeed_TypeDef;

typedef enum
{ My_GPIO_Mode_AIN = 0x0,
  My_GPIO_Mode_IN_FLOATING = 0x04,
  My_GPIO_Mode_IPD = 0x28,
  My_GPIO_Mode_IPU = 0x48,
  My_GPIO_Mode_Out_OD = 0x14,
  My_GPIO_Mode_Out_PP = 0x10,
  My_GPIO_Mode_AF_OD = 0x1C,
  My_GPIO_Mode_AF_PP = 0x18
}My_GPIOMode_TypeDef;


typedef struct
{	uint16_t My_GPIO_Pin;                                       
  My_GPIOSpeed_TypeDef My_GPIO_Speed;                                
  My_GPIOMode_TypeDef My_GPIO_Mode;                                
}My_GPIO_InitTypeDef;


//**************************************************FUNCTION PROTOTYPE************************************************************

void GPIO_Config(void);
void Delay(__IO uint32_t ms);
void Toggle_Pin(My_GPIO_REG_TYPE* GPIOx, uint32_t GPIO_Pin);
void ClockSource(uint32_t Port, My_GPIO_REG_TYPE* GPIOx ,My_GPIO_InitTypeDef* GPIO_InitStruct, My_RRC_REG_TYPE* RCCx);
void GPIO_Reset_Pin(My_GPIO_REG_TYPE* GPIOx, uint32_t GPIO_Pin);
void GPIO_Set_Pin(My_GPIO_REG_TYPE* GPIOx, uint32_t GPIO_Pin);

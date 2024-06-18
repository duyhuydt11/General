//**************************************************INCLUDE***********************************************************
#include "thuvien_1.h"

//**************************************************FUNCTION***********************************************************

void Toggle_Pin(My_GPIO_REG_TYPE* GPIOx, uint32_t GPIO_Pin)
{
	if(GPIOx->My_ODR == GPIO_Pin)
		GPIOx->My_BSRR = GPIO_Pin << 16;
	else GPIOx->My_BSRR = GPIO_Pin;
}


void ClockSource(uint32_t Port, My_GPIO_REG_TYPE* GPIOx ,My_GPIO_InitTypeDef* GPIO_InitStruct, My_RRC_REG_TYPE* RCCx)
{
	static int loop = 0;
	if(loop < 1){
		GPIOx->My_CRH &= 0x44444444;
		GPIOx->My_CRL &= 0x44444444;
		loop++;
	}
	uint32_t Mode, pos = 0;
	RCCx->My_RCC_APB2ENR |= Port; 
	
	Mode = (uint32_t)((GPIO_InitStruct->My_GPIO_Mode) & (uint32_t)(0x0F)) | (GPIO_InitStruct->My_GPIO_Speed);
	// xac dinh vi tri pin
	if(GPIO_InitStruct->My_GPIO_Pin > GPIO_Pin_7)
	{
		for(uint16_t x = 0; x < 8; x++)
		{
			pos = 1 << (8+x);
			if(pos == (GPIO_InitStruct->My_GPIO_Pin))
			{
				pos = x;
				break;
			}
		}
		GPIOx->My_CRH |=  Mode << (4*pos);
	}
	
		
	else if (GPIO_InitStruct->My_GPIO_Pin <= GPIO_Pin_7)
	{
		for(uint16_t x = 0; x < 8; x++)
			{
				pos = 1 << x;
				if(pos == (GPIO_InitStruct->My_GPIO_Pin))
				{
					pos = x;
					break;
				}
			}
		GPIOx->My_CRL |=  Mode << (4*pos);
	}
}


void GPIO_Config(void)
{
	My_GPIO_InitTypeDef GPIO_InitStruct = {0};
	// cau hinh chan

	GPIO_InitStruct.My_GPIO_Mode = My_GPIO_Mode_Out_PP;
	GPIO_InitStruct.My_GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.My_GPIO_Speed = My_GPIO_Speed_50MHz;
	ClockSource(RCC_APB2Periph_GPIOC, My_GPIOC ,&GPIO_InitStruct, My_RCC);
	

	GPIO_InitStruct.My_GPIO_Mode = My_GPIO_Mode_IPD;
	GPIO_InitStruct.My_GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.My_GPIO_Speed = My_GPIO_Input_Mode;
	ClockSource(RCC_APB2Periph_GPIOB, My_GPIOB ,&GPIO_InitStruct, My_RCC);
	
//	GPIO_InitStruct.My_GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2
//																|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5
//																|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9
//																|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
//  GPIO_InitStruct.My_GPIO_Mode = My_GPIO_Mode_AIN;
//	GPIO_InitStruct.My_GPIO_Speed = My_GPIO_Input_Mode;
//  ClockSource(RCC_APB2Periph_GPIOB, My_GPIOB ,&GPIO_InitStruct, My_RCC);
//	
//	GPIO_InitStruct.My_GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2
//																|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5
//																|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9
//																|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_14|GPIO_Pin_15;
//  GPIO_InitStruct.My_GPIO_Mode = My_GPIO_Mode_AIN;
//	GPIO_InitStruct.My_GPIO_Speed = My_GPIO_Input_Mode;
//  ClockSource(RCC_APB2Periph_GPIOC, My_GPIOC ,&GPIO_InitStruct, My_RCC);
}

void GPIO_Reset_Pin(My_GPIO_REG_TYPE* GPIOx, uint32_t GPIO_Pin)
{
		GPIOx->My_BSRR = GPIO_Pin << 16;
}

void GPIO_Set_Pin(My_GPIO_REG_TYPE* GPIOx, uint32_t GPIO_Pin)
{
		GPIOx->My_BSRR = GPIO_Pin ;
}

void Delay(__IO uint32_t ms)
{
	for(uint32_t x = 0; x <= ms; x++)
		for(uint32_t y = 0; y <= 1200*ms/150; y++);
}

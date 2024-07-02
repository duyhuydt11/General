/************************************************************************************************************************************************/
																										//INCLUDE
/***********************************************************************************************************************************************/

#include "thuvien_1.h"

/************************************************************************************************************************************************/
																										//CONFIG FUNTION
/***********************************************************************************************************************************************/

// Calculate Pin_Number in uint type
uint8_t Pin_Numerical_Order(uint32_t Pin) 
{
    uint8_t Pin_Number = 0;
    while (Pin > 0) {
        Pin_Number++;
        Pin >>= 1;  // D?ch ph?i 1 bit
    }
    return (Pin_Number - 1);
}


// Port config
void ClockSource(uint32_t RCC_Port, My_GPIO_REG_TYPE* GPIOx ,My_GPIO_InitTypeDef* GPIO_InitStruct)
{
	uint32_t Mode = 0, Pos = 0, Step = 0, Temp = 0;
	static int loop = 0;
	
	if(loop < 1){
		GPIOx->My_CRH = 0x44444444;
		GPIOx->My_CRL = 0x44444444;
		GPIOx->My_ODR = 0x0;
		GPIOx->My_IDR = 0x0;
		loop++;
	}

	
	// provide clock pulse corresponding port 
	switch(RCC_Port)
	{
		case RCC_APB2Periph_AFIOA:
		case RCC_APB2Periph_AFIOB:
		case RCC_APB2Periph_AFIOC:
		case RCC_APB2Periph_AFIOD:
		case RCC_APB2Periph_AFIOE:
		case RCC_APB2Periph_AFIOF:
		case RCC_APB2Periph_AFIOG:
			My_RCC->My_RCC_APB2ENR |= RCC_APB2ENR_AFIOEN;
			My_RCC->My_RCC_APB2ENR |= RCC_Port - (uint32_t)0x01; 
			break;
		case RCC_APB2Periph_GPIOA:
		case RCC_APB2Periph_GPIOB:
		case RCC_APB2Periph_GPIOC:
		case RCC_APB2Periph_GPIOD:
		case RCC_APB2Periph_GPIOE:
		case RCC_APB2Periph_GPIOF:
		case RCC_APB2Periph_GPIOG:
			My_RCC->My_RCC_APB2ENR |= RCC_Port; 
			break;
	}
	
	// config depend on pull register of pin
	switch(GPIO_InitStruct->My_GPIO_Pull)
	{
		case My_PU:
			GPIOx->My_ODR |= GPIO_InitStruct->My_GPIO_Pin;
			break;
		
		case My_PD:
			GPIOx->My_ODR &= ~(GPIO_InitStruct->My_GPIO_Pin);
			break;
		
		case My_NoPull:
			break;
	}
	
	// config mode of pin
	switch(GPIO_InitStruct->My_GPIO_Mode)
	{
		case My_GPIO_Mode_Input_IT:
			Mode = (uint32_t)((GPIO_InitStruct->My_GPIO_Mode - (uint32_t)0x01) & (uint32_t)(0x0F)) | (GPIO_InitStruct->My_GPIO_Speed);
			break;
		case My_GPIO_Mode_Input:
		case My_GPIO_Mode_Out_PP:
		case My_GPIO_Mode_AF_OD:
		case My_GPIO_Mode_AF_PP:
		case My_GPIO_Mode_Out_OD:
		case My_GPIO_Mode_AIN:
		case My_GPIO_Mode_IN_FLOATING:
			Mode = (uint32_t)((GPIO_InitStruct->My_GPIO_Mode) & (uint32_t)(0x0F)) | (GPIO_InitStruct->My_GPIO_Speed);
			break;
	}
	
	
	
	Step = (GPIO_InitStruct->My_GPIO_Pin > GPIO_Pin_7) ? (8) : (0);
	
	
	// Process config gpio
	for(uint32_t x = 0; x < 8; x++)
		{
			Temp = 1 << (Step+x);
			if(Temp == (GPIO_InitStruct->My_GPIO_Pin))
			{
				Pos = x;
				break;
			}
		}
	(GPIO_InitStruct->My_GPIO_Pin > GPIO_Pin_7) ? (GPIOx->My_CRH &= ~(0x0Fu << (4*Pos))) : (GPIOx->My_CRL &= ~(0x0Fu << (4*Pos)));
	(GPIO_InitStruct->My_GPIO_Pin > GPIO_Pin_7) ? (GPIOx->My_CRH |=  Mode << (4*Pos)) : (GPIOx->My_CRL |=  (Mode) << (4*Pos));
}

// Config Priority of Interupr
void External_Interupt_Setup_Priority(uint16_t PreemptPriority, uint16_t SubPriority, IRQn_Type IRQn)
{
	uint32_t prioritygroup = 0x00U;
	prioritygroup = NVIC_GetPriorityGrouping();
	NVIC_SetPriority(IRQn, NVIC_EncodePriority(prioritygroup, PreemptPriority, SubPriority));
	NVIC_EnableIRQ(IRQn);
}


// Config Interupt in Pin and Line
void External_Interupt_Setup_Config(My_AFIO_Exitr_Port AFIO_Port, uint32_t GPIO_Pin, My_ExItr_Mode_TypeDef Mode)
{
	uint8_t Pin = Pin_Numerical_Order(GPIO_Pin);
	uint8_t Pos = (Pin % 4);
	uint8_t Icr_Pos = (Pin/4);
	My_AFIO->My_EXTICR[Icr_Pos] |= AFIO_Port << (uint8_t)(Pos*4); 
	// set mode for GPIO pin
	switch(Mode)
	{
		case My_IT_Mode_It_Input_Rising:
			My_EXTI->My_RTSR &= ~GPIO_Pin;
			My_EXTI->My_RTSR |= GPIO_Pin;
			break;
		
		case My_IT_Mode_It_Input_Falling:
			My_EXTI->My_FTSR &= ~GPIO_Pin;
			My_EXTI->My_FTSR |= GPIO_Pin;
			break;
		
		case My_IT_Mode_It_Input_Rising_Falling:
			My_EXTI->My_RTSR &= ~GPIO_Pin;
			My_EXTI->My_RTSR |= GPIO_Pin;
			My_EXTI->My_FTSR &= ~GPIO_Pin;
			My_EXTI->My_FTSR |= GPIO_Pin;
			break;
	}
	
		My_EXTI->My_IMR |= 1u << Pin;
		
}


void GPIO_Config(void)
{
	My_GPIO_InitTypeDef GPIO_InitStruct = {0};
	// cau hinh chan
	
	// INPUT OUTPUT
	GPIO_InitStruct.My_GPIO_Mode = My_GPIO_Mode_Out_PP;
	GPIO_InitStruct.My_GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.My_GPIO_Speed = My_GPIO_Speed_50MHz;
	GPIO_InitStruct.My_GPIO_Pull = My_NoPull;
	ClockSource(RCC_APB2Periph_GPIOC, My_GPIOC ,&GPIO_InitStruct);
	
	// EXTERNAL INTERUPT
	GPIO_InitStruct.My_GPIO_Mode = My_GPIO_Mode_Input_IT;
	GPIO_InitStruct.My_GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStruct.My_GPIO_Speed = My_GPIO_Input_Mode;
	GPIO_InitStruct.My_GPIO_Pull = My_NoPull;
	ClockSource(RCC_APB2Periph_AFIOB, My_GPIOB ,&GPIO_InitStruct);
	External_Interupt_Setup_Config(My_AFIO_Port_B, GPIO_Pin_4,My_IT_Mode_It_Input_Rising);
	External_Interupt_Setup_Priority(0,0, EXTI4_IRQn);
	
	GPIO_InitStruct.My_GPIO_Mode = My_GPIO_Mode_Input;
	GPIO_InitStruct.My_GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.My_GPIO_Speed = My_GPIO_Input_Mode;
	GPIO_InitStruct.My_GPIO_Pull = My_PU;
	ClockSource(RCC_APB2Periph_GPIOB, My_GPIOB ,&GPIO_InitStruct);

	
}

// Delay Function
void Delay(__IO uint32_t ms)
{
	for(uint32_t x = 0; x <= ms; x++)
		for(uint32_t y = 0; y <= 1200*ms/150; y++);
}


/************************************************************************************************************************************************/
																										//FUNTION IN/OUT SET/RESET BIT
/***********************************************************************************************************************************************/
void GPIO_Reset_Pin(My_GPIO_REG_TYPE* GPIOx, uint32_t GPIO_Pin)
{
		GPIOx->My_BSRR = GPIO_Pin << 16;
		GPIOx->My_BSRR &= ~GPIO_Pin;
}

void GPIO_Set_Pin(My_GPIO_REG_TYPE* GPIOx, uint32_t GPIO_Pin)
{
		GPIOx->My_BSRR = GPIO_Pin ;
		GPIOx->My_BSRR &= ~(GPIO_Pin << 16);
}

void GPIO_Toggle_Pin(My_GPIO_REG_TYPE* GPIOx, uint32_t GPIO_Pin)
{
	if(GPIOx->My_ODR == GPIO_Pin)
		GPIOx->My_BSRR = GPIO_Pin << 16;
	else GPIOx->My_BSRR = GPIO_Pin;
}


uint32_t GPIO_Read_Pin(My_GPIO_REG_TYPE* GPIOx, uint32_t GPIO_Pin)
{
	if((GPIOx->My_IDR & GPIO_Pin) != (uint32_t)GPIO_Pin_Reset)
		return GPIO_Pin_Set;
	else return GPIO_Pin_Reset;

}

// Read interupt on Pin
uint32_t GPIO_Read_Interupt_Line(uint32_t Line)
{
		if(((My_EXTI->My_PR)&(1 << Line)) != 0u) // pull down
		{
			return GPIO_Pin_Set;
		}
		else return GPIO_Pin_Reset;
}

// Erase Interupt on Pin
void GPIO_Erase_Interupt_Line(uint32_t Line)
{
		if(GPIO_Read_Interupt_Line(Line))
			My_EXTI->My_PR |= 1 << Line;
}

/************************************************************************************************************************************************/
																										//DEBOUNCE_HAREWARE_BUTTON
/***********************************************************************************************************************************************/
uint32_t GPIO_Button_Pin(My_GPIO_REG_TYPE* GPIOx, uint32_t GPIO_Pin, My_Pull_Mode Mode_Pull)
{
	uint8_t Mode = (Mode_Pull == My_PD) ? 0u : 1u;
	if((GPIOx->My_IDR & GPIO_Pin) != (uint32_t)Mode)
	{
		while((GPIOx->My_IDR & GPIO_Pin) != (uint32_t)Mode);
		return GPIO_Pin_Set;
	}
	
	else return GPIO_Pin_Reset;

}

/************************************************************************************************************************************************/
																										//DEBOUNCE_SOFTWARE_BUTTON
/***********************************************************************************************************************************************/
uint32_t Count_Debounce_Button(My_GPIO_REG_TYPE* GPIOx, uint32_t GPIO_Pin, My_Pull_Mode Mode_Pull)
{
	uint8_t Mode = (Mode_Pull == My_PD) ? 0u : 1u;
	// DEBOUNCE FOR 
	if((GPIOx->My_IDR & GPIO_Pin) == Mode)
	{
		Delay(20);
		if((GPIOx->My_IDR & GPIO_Pin) == Mode)
		{
			while((GPIOx->My_IDR & GPIO_Pin) == 0);
			return 1;
		}
	}
	return 0;
	
}

//uint32_t Hold_Debounce_Button(My_GPIO_REG_TYPE* GPIOx, uint32_t GPIO_Pin)
//{
//	if((GPIOx->My_IDR & GPIO_Pin) == (uint32_t)GPIO_Pin_Reset)
//	{
//		Delay(20);
//		while((GPIOx->My_IDR & GPIO_Pin) == (uint32_t)GPIO_Pin_Reset)
//		{
//			if
//		}
//		return 1;
//	}
//	return 0;
//}

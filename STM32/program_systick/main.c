#include "thuvien_1.h"
// systick o tren
void SysTick_Handler(void);
void EXTI4_IRQHandler(void);

int main()
{
	GPIO_Config();
	GPIO_Reset_Pin(My_GPIOC, GPIO_Pin_13);
	while(1)
	{
		GPIO_Toggle_Pin(My_GPIOC, GPIO_Pin_13);
		Delay(100);
	}

}
// Khong su dung dealy trong ngat duoc
void EXTI4_IRQHandler(void)
{
	if(GPIO_Read_Interupt_Line(4))
	{
			Systick_Delay_Interupt(100);
			GPIO_Erase_Interupt_Line(4);
	}

}

void SysTick_Handler(void)
{
	AddTick();
}

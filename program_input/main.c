#include "thuvien_1.h"

int main()
{
	GPIO_Config();
	while(1)
	{
		if((My_GPIOB->My_IDR & GPIO_Pin_3) == GPIO_Pin_3)
		{
			GPIO_Set_Pin(My_GPIOC, GPIO_Pin_13);
		}
		else GPIO_Reset_Pin(My_GPIOC, GPIO_Pin_13);
		Delay(20);
	}

}

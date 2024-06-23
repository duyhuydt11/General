#include "thuvien_1.h"

int main()
{
	My_GPIOC->My_BSRR = 0;
	GPIO_Config();
	int count = 0;
	while(1)
	{
		
		if(GPIO_Read_Pin(My_GPIOB, GPIO_Pin_4) == 0) count++;
		if(count == 4)
			GPIO_Set_Pin(My_GPIOC, GPIO_Pin_13);
		
		else
		{
			GPIO_Reset_Pin(My_GPIOC, GPIO_Pin_13);
		}

		Delay(100);
	}

}

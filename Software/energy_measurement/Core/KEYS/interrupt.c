/*

��ʱ���ж����ڵ�

TIM3 10msһ���ж�

1.����������ȡ���������̰���
2.��Ļˢ�º���

*/

#include "interrupt.h" 
#include "screen.h"
#include "stm32g0xx_hal_uart.h"
#include "main.h" 
#include "usart.h" 
#include "hlw8032.h"

//����״̬���ṹ������
struct keys key[2]={0,0,0};

//����ȫ�ֱ���
extern struct SCR scra;
extern struct SCR scro;
extern uint8_t HLW_sta;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern uint32_t uart_tim_flag;
extern uint8_t uart_flag ;
extern bool dev_key;


//��ʱ���жϻص�����
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	
	//�ж��Ƿ�ʱ�������ж�
	if(htim->Instance ){
		
		//���尴������IO
		key[0].key_sta = HAL_GPIO_ReadPin (GPIOC ,GPIO_PIN_14 );
		key[1].key_sta = HAL_GPIO_ReadPin (GPIOC ,GPIO_PIN_15 );
		
		//�жϰ���״̬�����°���״̬��
		for(int i=0;i<2;i++)
		{
			switch (key[i].judge_sta )
			{
				case 0:
				{
					if(key[i].judge_sta==0) 
					{
						key[i].key_time =0;
						key[i].judge_sta =1;
					}
				}
				break;
				case 1:
				{
					if(key[i].key_sta ==0)
					{
						key[i].judge_sta =2;
					}
					else key[i].judge_sta =0;
				}
				break;
				case 2:
				{
					if(key[i].key_sta == 1)
					{
						key[i].judge_sta =0;
						if (key[i].key_time <60) key[i].s_flag =1;
						if (key[i].key_time >60) key[i].l_flag =1;
					}
					else key[i].key_time ++;
			//		if (key[i].key_time >60) key[i].l_flag =1;
				}
				break;
			}
		}
	}
	
	//��Ļ��ʱˢ��
	if(scra.page1 !=scro.page1 || scra.page2 !=scro.page2 || scra.page_flag !=scro.page_flag ){
		
		SCREEN_SW(scra.page_flag ,scra.page1 ,scra.page2);
		
		scro=scra;
		
	}
	
	//����õ���
	/*if(dev_key==1){
		
		
		
	}*/
	
	
}

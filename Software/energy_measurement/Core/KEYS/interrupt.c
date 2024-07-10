/*

定时器中断所在地

TIM3 10ms一次中断

1.包含按键读取（长按、短按）
2.屏幕刷新函数

*/

#include "interrupt.h" 
#include "screen.h"
#include "stm32g0xx_hal_uart.h"
#include "main.h" 
#include "usart.h" 
#include "hlw8032.h"

//按键状态机结构体数组
struct keys key[2]={0,0,0};

//引用全局变量
extern struct SCR scra;
extern struct SCR scro;
extern uint8_t HLW_sta;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern uint32_t uart_tim_flag;
extern uint8_t uart_flag ;
extern bool dev_key;


//定时器中断回调函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	
	//判断是否定时器触发中断
	if(htim->Instance ){
		
		//定义按键所在IO
		key[0].key_sta = HAL_GPIO_ReadPin (GPIOC ,GPIO_PIN_14 );
		key[1].key_sta = HAL_GPIO_ReadPin (GPIOC ,GPIO_PIN_15 );
		
		//判断按键状态并更新按键状态机
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
	
	//屏幕定时刷新
	if(scra.page1 !=scro.page1 || scra.page2 !=scro.page2 || scra.page_flag !=scro.page_flag ){
		
		SCREEN_SW(scra.page_flag ,scra.page1 ,scra.page2);
		
		scro=scra;
		
	}
	
	//检测用电器
	/*if(dev_key==1){
		
		
		
	}*/
	
	
}

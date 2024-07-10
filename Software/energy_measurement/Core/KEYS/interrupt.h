#ifndef __INTERRUPT_H
#define __INTERRUPT_H

#include "main.h"
#include "stm32g0xx_hal.h"
#include "stdio.h" 
#include "stdbool.h" 

//定义按键状态机结构体
struct keys
{
	unsigned char judge_sta;
	bool key_sta;
	bool l_flag;
	bool s_flag;
	uint16_t key_time;
};

//声明定时器中断回调函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif
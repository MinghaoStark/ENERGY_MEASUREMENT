#ifndef __SCREEN_H
#define __SCREEN_H

#include "oled.h"
#include "main.h"
#include "stm32g0xx_hal.h"

void SCREEN_Start(void);//开始界面
void SCREEN_First_ZH(void);//中文界面1
void SCREEN_First_EN(void);//英文界面1

//屏幕切换函数
void SCREEN_SW(unsigned char x,unsigned char y,unsigned char z);

//屏幕显示序号
void SCREEN_NO(uint8_t x);

#endif
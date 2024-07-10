#ifndef __SCREEN_H
#define __SCREEN_H

#include "oled.h"
#include "main.h"
#include "stm32g0xx_hal.h"

void SCREEN_Start(void);//��ʼ����
void SCREEN_First_ZH(void);//���Ľ���1
void SCREEN_First_EN(void);//Ӣ�Ľ���1

//��Ļ�л�����
void SCREEN_SW(unsigned char x,unsigned char y,unsigned char z);

//��Ļ��ʾ���
void SCREEN_NO(uint8_t x);

#endif
#include "oled.h"
#include "stdlib.h"
#include "main.h"


//开始界面
void SCREEN_Start(void){
	OLED_Clear();
	OLED_ShowString(0,0,(unsigned char *)"ElectronicManagement",12);
	OLED_ShowString(0,16,(unsigned char *)"       System",12);
	OLED_ShowString(0,32,(unsigned char *)" Designed by wuneng",12);	
	OLED_Refresh();
}


//设备编号，功率，功率因数，电流，电压（中文）
void SCREEN_First_ZH(void){
	OLED_Clear();
	
	//设备编号
	OLED_ShowChinese(0,0,7,16);
	OLED_ShowChinese(16,0,8,16);
	OLED_ShowChinese(32,00,9,16);
	OLED_ShowChinese(48,00,10,16);
	
	//功率因数
	OLED_ShowChinese(0,36,0,16);
	OLED_ShowChinese(16,36,1,16);
	OLED_ShowChinese(32,36,2,16);
	OLED_ShowChinese(48,36,3,16);
	
	//电流
//	OLED_ShowChinese(0,18,4,16);
//	OLED_ShowChinese(16,18,5,16);
	
	//电压
//	OLED_ShowChinese(0,36,4,16);
//	OLED_ShowChinese(16,36,6,16);

	//功率
	OLED_ShowChinese(0,18,0,16);
	OLED_ShowChinese(16,18,1,16);
	
	OLED_Refresh();
}


//电流，电压，功率，功率因数（英文）
void SCREEN_First_EN(void){
	OLED_Clear();
	OLED_ShowString(0,0,(unsigned char *)"Device ID",12);
	OLED_ShowString(0,14,(unsigned char *)"Vol       Cur",12);
	OLED_ShowString(0,28,(unsigned char *)"Pow       Fac",12);
	OLED_ShowString(0,42,(unsigned char *)" ",12);
	OLED_Refresh();
}


void SCREEN_NO(uint8_t x){
	OLED_Clear();
	OLED_ShowNum(0,0,x+1,1,16);
	OLED_Refresh();
}

//屏幕刷新
void SCREEN_SW(bool x,uint8_t y,uint8_t z){
	
if (x==0){
	switch (y){
		case 0:SCREEN_First_EN();
		break ;
		case 1:SCREEN_First_ZH();
		break ;
	}
}
else {
	SCREEN_NO(z);
	switch (z){
		case 0:OLED_ShowString(16,16,(unsigned char *)"Resistance",16);
		break ;
		case 1:OLED_ShowString(16,16,(unsigned char *)"iPhone_Charge",16);
		break ;
		case 2:OLED_ShowString(16,16,(unsigned char *)"TabS9_Charge",16);
		break ;
		case 3:OLED_ShowString(16,16,(unsigned char *)"Laptop_Charge",16);
		break ;
		case 4:OLED_ShowString(16,16,(unsigned char *)"Resistance",16);
		break ;
		case 5:OLED_ShowString(16,16,(unsigned char *)"Resistance",16);
		break ;
		case 6:OLED_ShowString(16,16,(unsigned char *)"Heater",16);
		break ;
	}
	OLED_Refresh();
}
	
}




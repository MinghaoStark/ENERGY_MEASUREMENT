/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "oled.h"
#include "screen.h"
#include "interrupt.h"
#include "hlw8032.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */




/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t scr;

//定义屏幕刷新状态机
struct SCR scra={0,0,0};//新状态
struct SCR scro={0,0,0};//旧状态

//定义设备检测状态机
bool dev_key =0;

//全局调用按键结构体
extern struct keys key[];

//数据处理进度状态机
uint8_t HLW_sta=0;

//串口1
uint8_t Resbuf[48]; //串口1接收缓冲区
uint8_t RawData[24]; //缓冲区中读出整组数据
uint16_t Receive; //串口读取临时数据

uint8_t uart_flag =100;//有效数据在缓冲区中位置，100表示未找到有效数据

//电参数
float V =0; //有效电压
double C =0;//有效电流
double P =0;//有功功率
double E_CON =0;
double AP =0;//视在功率
double PF =0;//功率因数

//声明电参数寄存器结构体
struct REG reg={0,0,0,0,0,0} ;

//设备特征
struct DEV device[7];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	
	
	//读入设备特征量
	device[0].pow = 5;//1.最小电流用电器（开关电源串电阻）
	device[0].fac = 50;
	device[0].ps = 2;
	device[0].fs = 10;
	device[1].pow = 8;//2.iPhone
	device[1].fac = 41;
	device[1].ps = 2;
	device[1].fs = 2;
	device[2].pow = 31;//3.平板电脑
	device[2].fac = 54;
	device[2].ps = 2;
	device[2].fs = 2;
	device[3].pow = 40;//4.笔记本电脑
	device[3].fac = 50;
	device[3].ps = 10;
	device[3].fs = 10;
	device[4].pow = 35;//5.路由器
	device[4].fac = 50;
	device[4].ps = 10;
	device[4].fs = 10;
	device[5].pow = 35;//6.台式机电脑
	device[5].fac = 50;
	device[5].ps = 10;
	device[5].fs = 10;
	device[6].pow = 1200;//7.最大功率用电器（烧水壶）
	device[6].fac = 95;
	device[6].ps = 200;
	device[6].fs = 10;

	
	//初始化屏幕状态机
	scra.page_flag =0;
	scra.page1 =0;
	scro.page_flag =0;
	scro.page1 =0;
	
	//初始化定时器中断
	HAL_TIM_Base_Start_IT (&htim3 );
	printf("TIM_IT_ON \r\n");
	
	//初始化OLED显示
	OLED_Init();
  SCREEN_Start();
	HAL_Delay(2000);
  OLED_Clear();
	SCREEN_First_EN();
	OLED_Refresh();
	
	printf("OLED_ON \r\n");
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

		GetData();
		
		SerchData();
		
		//转移数据包
		if(uart_flag<=24){
			for(int i=0;i<=23;i++){
			  RawData [i]=Resbuf [uart_flag+i];
		  }
		}
		uart_flag=100;
		
		//分离寄存器值
		RawTrans();
		
		ProcessData();
		
		//InputData();
		
		//按键事件_刷新屏幕
		if(key [0].s_flag ==1)
		{
			SCRPP();
			key [0].s_flag =0;
		}
		if(key [0].l_flag ==1)
		{
			if (scra.page_flag ==0) scra.page_flag =1;
			else scra.page_flag =0;
			key [0].l_flag =0;
		}
		
		//按键事件，用电器检测
		if(key [1].s_flag ==1)
		{
			//单击开始检测
			dev_key=1;
			key [1].s_flag =0;
			int comp =Compare();
			OLED_ShowNum(70,0,comp+1,3,12);
			OLED_Refresh();
		}
		
		
		
		if(key [0].l_flag ==1)
		{
			if (scra.page_flag ==0) scra.page_flag =1;
			else scra.page_flag =0;
			key [0].l_flag =0;
		}
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

//屏幕循环显示状态机更新函数
void SCRPP(void){
	if (scra.page_flag ==0){
		if (scra.page1 ==1) {
			scra.page1 =0;
		}
		else {
			scra.page1 =1;
		}
	}
	else {
		if (scra.page2 ==6) {
			scra.page2 =0;
		}
		else {
			scra.page2 ++;
		}
	}
}

//串口2重定向函数
int fputc(int ch, FILE *f)
 
{
 
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xffff);
 
  return ch;
 
}



/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
	
	
	
	
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

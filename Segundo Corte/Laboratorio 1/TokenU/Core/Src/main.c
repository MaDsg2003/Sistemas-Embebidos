/* USER CODE BEGIN Header */
/**
  **************************
  * @file           : main.c
  * @brief          : Main program body
  **************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  **************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include <stdio.h>
//==Comandos LCD==//
#define linea1 0x80
#define linea2 0xC0
#define clear  0x01
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
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */
unsigned long token =0;
unsigned long token2=0;
unsigned long token3=0;
char tex[8];
static char clave[8];
static char Hora[8];
static char OK[2]="OK";
static int temp=0, activar=0, activar2=0;
int i=0;
/*uint8_t contador=0;
uint8_t texto[32];
uint8_t ap[10]={0,1,2,3,4,5,6,7,8,9};*/
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
void initLCD();
void comando(unsigned char cmd, unsigned char rs);
void E_cadena(unsigned char *pal);
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
  MX_TIM2_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */
  //HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
  initLCD();

  E_cadena("Esperando  clave");
  comando(linea2, 0);
  E_cadena("Sin  sincronizar");
  while(activar == 0){
    }
  HAL_TIM_Base_Start_IT(&htim2);
  comando(clear,0);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1){
	  /*if(temp==0){
		  E_cadena("Su token es: ");
		  comando(linea2,0);
		  E_cadena(utoa(token3, tex, 16));
		  temp=2;
	  }else if (temp==1){
		  token2=token2+30;
		  token3=token2^token;
		  //E_cadena(utoa(token, tex, 16));
		  temp=0;
		  comando(clear,0);
	  }*/
	  HAL_Delay(2000);
	  CDC_Transmit_FS(OK, strlen(OK));
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 144;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 36000;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 60000-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, E_Pin|RS_Pin|D4_Pin|D5_Pin
                          |D6_Pin|D7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : E_Pin RS_Pin D4_Pin D5_Pin
                           D6_Pin D7_Pin */
  GPIO_InitStruct.Pin = E_Pin|RS_Pin|D4_Pin|D5_Pin
                          |D6_Pin|D7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void comando(unsigned char cmd, unsigned char rs){
	unsigned char t1,t2,i;
	HAL_GPIO_WritePin(GPIOB, RS_Pin, rs);
	t1=(cmd>>4)&0x0F;
	for(i=0;i<4;i++){
		t2=t1&0x01;
		switch(i){
		case 0:
			HAL_GPIO_WritePin(GPIOB, D4_Pin, t2);
		break;
		case 1:
			HAL_GPIO_WritePin(GPIOB, D5_Pin, t2);
		break;
		case 2:
			HAL_GPIO_WritePin(GPIOB, D6_Pin, t2);
		break;
		case 3:
			HAL_GPIO_WritePin(GPIOB, D7_Pin, t2);
		break;
		}
		t1=t1>>1;
	}
	HAL_GPIO_WritePin(GPIOB, E_Pin, 1);
		HAL_Delay(2);
	HAL_GPIO_WritePin(GPIOB, E_Pin, 0);
		HAL_Delay(1);
	t1=cmd&0x0F;
	for(i=0;i<4;i++){
			t2=t1&0x01;
		switch(i){
		case 0:
			HAL_GPIO_WritePin(GPIOB, D4_Pin, t2);
		break;
		case 1:
			HAL_GPIO_WritePin(GPIOB, D5_Pin, t2);
		break;
		case 2:
			HAL_GPIO_WritePin(GPIOB, D6_Pin, t2);
		break;
		case 3:
			HAL_GPIO_WritePin(GPIOB, D7_Pin, t2);
		break;
		}
			t1=t1>>1;
		}
		HAL_GPIO_WritePin(GPIOB, E_Pin, 1);
			HAL_Delay(2);
		HAL_GPIO_WritePin(GPIOB, E_Pin, 0);
			HAL_Delay(1);
}

void initLCD(){
		HAL_Delay(40);
	comando(0x02,0);
		HAL_Delay(2);
	comando(0x28,0);
		HAL_Delay(2);
	comando(0x01,0);
		HAL_Delay(2);
	comando(0x0C,0);
		HAL_Delay(2);
	comando(0x06,0);
		HAL_Delay(2);
	comando(0x80,0);
		HAL_Delay(100);
}

void E_cadena(unsigned char *pal){
	while(*pal){
		comando(*pal,1);
		pal++;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
	temp=1;
}

void Receive(uint8_t* Buf, uint32_t Len){

	activar2++;
	/*if(Buf[0]=='s' || Buf[0]=='S'){
		activar=1;
		//CDC_Transmit_FS(OK, strlen(OK));
	}else if(Buf[0]=='H' && Buf[1]=='O'){
		for(i=0; i<9; i++){
			Hora[i]=Buf[i+2];
		}
		token2=strtoul(Hora, NULL, 10);
		//CDC_Transmit_FS(OK, strlen(OK));
	}else if(Buf[0]=='K' && Buf[1]=='E'){
		for(i=0; i<9; i++){
			clave[i]=Buf[i+2];
		}

		token=strtoul(clave, NULL, 16);
		//CDC_Transmit_FS(OK, strlen(OK));
	}*/CDC_Transmit_FS(OK, strlen(OK));
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

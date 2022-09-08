/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
//Comandos para operaciones del lcd
#define linea1 0x80 //Direccion del cursor en linea1
#define linea2 0xC0 //Direccion del cursos en linea2
#define clear  0x01 //Comando que borra lso datos que esta impirmiendo el lcd
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
void initLCD(); //Funcion que inicializa el lcd
void comando(unsigned char cmd, unsigned char rs); //Funcion para enviar la informacion al lcd
void E_cadena(unsigned char *pal); //Funcion que nos ayuda a imprimir un string o cadena
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//unsigned char pal[]="123456";
unsigned long cont=0; //Contador para llevar los segundos en el microcontrolador
unsigned long token=0xF49DC57D; //token asignado por nosotros
unsigned long token2=30000; // Token inicializado que cambaira cada 30segundos
unsigned long clave=0; //calve resultante que sera imprimida en el lcd
char pal[9],i; //Array que me ayudara a convertir a calve en string
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
  /* USER CODE BEGIN 2 */
  initLCD();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  HAL_Delay(1);
  E_cadena("Su token es: ");
  HAL_Delay(1);
  comando(linea2, 0);
  HAL_Delay(15);
//While para poder sincronizar el microcontrolador con el programa del pc
  while(i<2){
     if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9 )==GPIO_PIN_RESET){ //evaluamos la fotoresistencia
	 		 i=4;
	 	  }else{
	 		 i=0;
	 	  }
  }
  clave=token^token2;//realizamos el primer XOR de nuestro token con el primier intervalo de tiempo
  E_cadena(itoa(clave, pal, 16)); //itoa se actualizo en stdlib.h ya que esta no contaba con ultoa

  while (1)
  {
	  HAL_Delay(1);
	  cont++; //contador que nos define los milisegundso gastados
	  if(cont==14960){//se evalua en esto para poder borrar los datos del lcd y no sobreescribir en el
		  //se realiza unos milisegundos antes de los 30seg para no retrasar el microcontrolador
		  comando(clear,0);
		  HAL_Delay(1);
		  E_cadena("Su token es: ");
		  HAL_Delay(1);
		  comando(linea2, 0);
	  }else if(cont==14980){ //15000 dura aproximada 30segundos
		  token2=token2+30000;//actualizamos el valor del token 2 comparador
		  clave=token^token2;//realizamos el XOR con token2 actualizado
		  E_cadena(itoa(clave, pal, 16));//imprimimos el resutlado de calve el pal
		  cont=0;
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
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
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, E_Pin|RS_Pin|D4_Pin|D5_Pin
                          |D6_Pin|D7_Pin|LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : E_Pin RS_Pin D4_Pin D5_Pin
                           D6_Pin D7_Pin LED_Pin */
  GPIO_InitStruct.Pin = E_Pin|RS_Pin|D4_Pin|D5_Pin
                          |D6_Pin|D7_Pin|LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : Foto_Pin */
  GPIO_InitStruct.Pin = Foto_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Foto_GPIO_Port, &GPIO_InitStruct);

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
		HAL_Delay(5);
}

void E_cadena(unsigned char *pal){
	while(*pal){
		comando(*pal,1);
		pal++;
	}
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

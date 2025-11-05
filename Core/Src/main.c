/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "cmsis_os.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>

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
//uint8_t *ascii1 =0;
//uint8_t *ascii2 =0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */
uint16_t HexToASCII(uint8_t data_hex);
//void uint8_to_ascii(uint8_t data, uint8_t *ascii1, uint8_t *ascii2);
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
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  // start menu
  uint8_t startstr[] = "Demo6_1:Priority inversion.\r\n";
  HAL_UART_Transmit(&huart3,startstr,sizeof(startstr),0xFFFF);

  uint8_t startstr1[] = "Using binary semaphore.\r\n";
  HAL_UART_Transmit(&huart3,startstr1,sizeof(startstr1),0xFFFF);

  // transmit 32bit BaudRate，process low 16b only.
  uint8_t startstr2[] = "Baud Rate = ";
  HAL_UART_Transmit(&huart3,startstr2,sizeof(startstr2),0xFFFF);

  uint16_t high8Bits = HexToASCII((huart3.Init.BaudRate>>8) & 0xFF); 		//high 8bit
  uint16_t low8Bits = HexToASCII(huart3.Init.BaudRate & 0xFF); 				//low 8bit

  HAL_UART_Transmit(&huart3,&high8Bits,2,100);										//transmit high 8 bit
  HAL_UART_Transmit(&huart3,&low8Bits,2,100);										//transmit low 8 bit

  uint8_t startstr3[] = "\r\n";
  HAL_UART_Transmit(&huart3,startstr3,sizeof(startstr3),0xFFFF);
  HAL_Delay(10);

  //or
  printf("Baud Rate = %ld.\r\n\r\n",huart3.Init.BaudRate);

  uint8_t startstr4[] = "1. Connect to PC via USART3.\r\n";
  HAL_UART_Transmit(&huart3,startstr4,sizeof(startstr4),0xFFFF);

  uint8_t startstr5[] = "2. View result on PC via ComAssistant.\r\n\r\n";
  HAL_UART_Transmit(&huart3,startstr5,sizeof(startstr5),0xFFFF);

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* Call init function for freertos objects (in cmsis_os2.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
int __io_putchar(int ch)
{
	HAL_UART_Transmit(&huart3,(uint8_t*)&ch,1,0xFFFF);
	return ch;
}

//将单个字节转化为两个ASCII字符进行显示，即一个字节转化为两个字节。
//如0x27，转化为'2' '7'两个字符。
uint16_t HexToASCII(uint8_t data_hex)
{
    uint8_t data_ASCII_H;
    uint8_t data_ASCII_L;
    uint16_t data_ASCII;

    data_ASCII_H = ((data_hex >> 4) & 0x0F);
    data_ASCII_L = data_hex & 0x0F;

    if(data_ASCII_H <= 9)
    {
        data_ASCII_H += 0x30;
    }
    else if((data_ASCII_H >= 10) && (data_ASCII_H <= 15))
    {
        data_ASCII_H += 0x37;
    }

    if(data_ASCII_L <= 9)
    {
        data_ASCII_L += 0x30;
    }
    else if((data_ASCII_L >= 10) && (data_ASCII_L <= 15))
    {
        data_ASCII_L += 0x37;
    }
    //HAL_UART_Transmit打印输出顺序是先低8b后高8b
    data_ASCII = (((data_ASCII_L & 0x00FF) << 8) | data_ASCII_H);

    return data_ASCII;
}

//void uint8_to_ascii(uint8_t data, uint8_t *ascii1, uint8_t *ascii2)
//{
//    uint8_t high_nibble = (data >> 4) & 0x0F;  	// 获取高四位
//    uint8_t low_nibble = data & 0x0F;          		// 获取低四位
//    // 将高四位和低四位转换为对应的 ASCII 字符
//    *ascii1 = (high_nibble <= 9) ? ('0' + high_nibble) : ('A' + (high_nibble - 10));
//    *ascii2 = (low_nibble <= 9) ? ('0' + low_nibble) : ('A' + (low_nibble - 10));
//}

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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


#include "main.h"
#include "cmsis_os.h"
#include<stdio.h>
#include"uart.h"
#include"adc.h"
#include"exti.h"


void SystemClock_Config(void);
static void MX_GPIO_Init(void);

#define TASK1_BIT (1UL<<0UL)
#define TASK2_BIT (1UL<<1UL)

EventGroupHandle_t xEventGroup;

static void vEventBitSettingTask(void *pvParameters);
static void vEventBitReadingTask(void *pvParameters);


int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  USART2_UART_TX_Init();

  printf("System Initializing...\n\r");

  xEventGroup = xEventGroupCreate();

  xTaskCreate(vEventBitSettingTask, "BitSetter", 100, NULL, 1, NULL);
  xTaskCreate(vEventBitReadingTask, "BitReader", 100, NULL, 1, NULL);

  vTaskStartScheduler();


  while (1)
  {
  }
}

static void vEventBitSettingTask(void *pvParameters)
{
	const TickType_t delayMS_500 = pdMS_TO_TICKS(500UL);

	while(1)
	{
		vTaskDelay(delayMS_500);
		printf("Bit setting task -\t about to set 0.\r\n");
		xEventGroupSetBits(xEventGroup,TASK1_BIT);

		vTaskDelay(delayMS_500);
		printf("Bit setting task -\t about to set 1.\r\n");
		xEventGroupSetBits(xEventGroup,TASK2_BIT);
	}
}

static void vEventBitReadingTask(void *pvParameters)
{
	EventBits_t xEventGroupValue;
	const EventBits_t xBitsToWaitFor = (TASK1_BIT | TASK2_BIT);
	const TickType_t delayMS_200   = pdMS_TO_TICKS(200UL);


	while(1)
	{
		xEventGroupValue = xEventGroupWaitBits(/*The event group to lead*/
				xEventGroup,
				xBitsToWaitFor,
				pdTRUE,
				pdFALSE,
				portMAX_DELAY);
		if((xEventGroupValue & TASK1_BIT) != 0)
				{
					printf("BIT READING TASK : EVENT BIT 0 SET - TASK1 \r\n");
				}

		if((xEventGroupValue & TASK2_BIT) != 0)
				{
					printf("BIT READING TASK : EVENT BIT 1 SET - TASK2 \r\n");
				}
		vTaskDelay(delayMS_200);
	}
}

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
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1)
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
#

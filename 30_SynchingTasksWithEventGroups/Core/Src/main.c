
#include "main.h"
#include "cmsis_os.h"
#include<stdio.h>
#include"uart.h"
#include"adc.h"
#include"exti.h"


void SystemClock_Config(void);
static void MX_GPIO_Init(void);

#define TASK1_BIT  (1U<<0U)
#define TASK2_BIT  (1U<<1U)
#define TASK3_BIT  (1U<<2U)

EventGroupHandle_t xEventGroup;

EventBits_t  uxAllSyncBits = ( TASK1_BIT | TASK2_BIT | TASK3_BIT);

static void Task1(void *pvParameters);
static void Task2(void *pvParameters);
static void Task3(void *pvParameters);


int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  USART2_UART_TX_Init();

  printf("System Initializing...\n\r");

  xEventGroup = xEventGroupCreate();

  xTaskCreate(Task1, "Task 1",128,NULL,1,NULL);
  xTaskCreate(Task2, "Task 2",128,NULL,1,NULL);
  xTaskCreate(Task3, "Task 3",128,NULL,1,NULL);


  vTaskStartScheduler();

  while (1)
  {
  }
}



static void Task1(void *pvParameters)
{
	EventBits_t uxReturn;

	while(1)
	{
		uxReturn = xEventGroupSync(
					xEventGroup,
					TASK1_BIT,
					uxAllSyncBits,
					portMAX_DELAY);
		if((uxReturn & uxAllSyncBits) == uxAllSyncBits)
		{
			//Do something....
		}
	}
}


static void Task2(void *pvParameters)
{
	EventBits_t uxReturn;

	while(1)
	{
		uxReturn = xEventGroupSync(
					xEventGroup,
					TASK2_BIT,
					uxAllSyncBits,
					portMAX_DELAY);
		if((uxReturn & uxAllSyncBits) == uxAllSyncBits)
		{
			//Do something....
		}
	}
}

static void Task3(void *pvParameters)
{
	EventBits_t uxReturn;

	while(1)
	{
		uxReturn = xEventGroupSync(
					xEventGroup,
					TASK3_BIT,
					uxAllSyncBits,
					portMAX_DELAY);
		if((uxReturn & uxAllSyncBits) == uxAllSyncBits)
		{
			//Do something....
			printf("All tasks set\r\n");
			for(int i=0;i<60000;i++);
		}
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

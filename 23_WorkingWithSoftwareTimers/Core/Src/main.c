
#include "main.h"
#include "cmsis_os.h"
#include<stdio.h>
#include"uart.h"
#include"adc.h"
#include"exti.h"



osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

void prvOneShotTimerCallback(TimerHandle_t xTimer);
void prvAutoReloadTimerCallback(TimerHandle_t xTimer);

//The periods assigned to the one-shot and auto-reload timers respectively
#define mainONE_SHOT_TIMER_PERIOD    (pdMS_TO_TICKS(4000UL))
#define mainAUTO_RELOAD_TIMER_PERIOD (pdMS_TO_TICKS(500UL))

TimerHandle_t xAutoReloadTimer, xOneShotTimer;
BaseType_t xTimer1Started, xTimer2Started;

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  USART2_UART_TX_Init();

  printf("System initiallizing...");

 xOneShotTimer = xTimerCreate("OneShot",
		 	 	 	 	 	 mainONE_SHOT_TIMER_PERIOD,
							 pdFALSE,
							 0,
							 prvOneShotTimerCallback);

 xAutoReloadTimer = xTimerCreate("AutoReload",
		 	 	 	 	 	 mainAUTO_RELOAD_TIMER_PERIOD,
							 pdTRUE,
							 0,
							 prvAutoReloadTimerCallback);

 xTimer1Startred = xTimerStart(xOneShotTimer,0);
 xTimer2Startred = xTimerStart(xAutoReloadTimer,0);

  while (1)
  {
  }

}

void prvOneShotTimerCallback(TimerHandle_t xTimer)
{
	static TickType_t xTimeNow;
	xTimeNow = xTaskGetTickCount();
	printf("One-shot timer callback executing : %ld \n\r",xTimeNow);
}

void prvAutoReloadTimerCallback(TimerHandle_t xTimer)
{
	static TickType_t xTimeNow;
		xTimeNow = xTaskGetTickCount();
		printf("Auto-Reload timer callback executing : %ld \n\r",xTimeNow);
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

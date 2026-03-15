
#include "main.h"
#include "cmsis_os.h"


UART_HandleTypeDef huart2;

osThreadId defaultTaskHandle;
osThreadId Thread1Handle;
osThreadId Thread2Handle;
osThreadId Thread3Handle;
osMutexId uart_mutexHandle;


void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
void StartDefaultTask(void const * argument);
void Thread1Func(void const * argument);
void Thread2Func(void const * argument);
void Thread3Func(void const * argument);

typedef uint32_t TaskProfiler;
TaskProfiler Thread1_Profiler, Thread2_Profiler,Thread3_Profiler, DefaultThread_Profiler;

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();


  osMutexDef(uart_mutex);
  uart_mutexHandle = osMutexCreate(osMutex(uart_mutex));

  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of Thread1 */
  osThreadDef(Thread1, Thread1Func, osPriorityNormal, 0, 128);
  Thread1Handle = osThreadCreate(osThread(Thread1), NULL);

  /* definition and creation of Thread2 */
  osThreadDef(Thread2, Thread2Func, osPriorityNormal, 0, 128);
  Thread2Handle = osThreadCreate(osThread(Thread2), NULL);

  /* definition and creation of Thread3 */
    osThreadDef(Thread3, Thread3Func, osPriorityNormal, 0, 128);
    Thread3Handle = osThreadCreate(osThread(Thread3), NULL);


  /* Start scheduler */
  osKernelStart();
  while (1)
  {
  }

}

void StartDefaultTask(void const * argument)
{

  for(;;)
  {
//	DefaultThread_Profiler++;
    osDelay(1);
  }

}


void Thread1Func(void const * argument)
{
 uint8_t message[35] = "Greetings fromm Thread1 \n\r";
  for(;;)
  {
//	  HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
	  xSemaphoreTake(uart_mutexHandle, portMAX_DELAY);
	  HAL_UART_Transmit(&huart2, message,35,10);
	  xSemaphoreGive(uart_mutexHandle);
//	Thread1_Profiler++;
    osDelay(1000);
  }
}


void Thread2Func(void const * argument)
{
	uint8_t message[35] = "Greetings fromm Thread2 \n\r";
  for(;;)
  {
//	Thread2_Profiler++;
	if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13) == GPIO_PIN_RESET)
	{
		  xTaskNotify(Thread2Handle,0x01,eSetBits);

		xSemaphoreTake(uart_mutexHandle, portMAX_DELAY);
		HAL_UART_Transmit(&huart2, message,35,10);
		xSemaphoreGive(uart_mutexHandle);	}



    osDelay(1000);
  }
}

void Thread3Func(void const * argument)
{
	uint8_t message[35] = "Greetings fromm Thread3 \n\r";
	uint32_t notifyValue;
  for(;;)
  {
	  xTaskNotifyWait(pdFALSE, 0xFF, &notifyValue, portMAX_DELAY);
	  if(notifyValue & 0x01 !=  0x00)
	  {
		  xSemaphoreTake(uart_mutexHandle, portMAX_DELAY);
		  HAL_UART_Transmit(&huart2, message,35,10);
		  xSemaphoreGive(uart_mutexHandle);
	  }


	  osDelay(1000);

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

static void MX_USART2_UART_Init(void)
{


  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
}


static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}




void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

  if (htim->Instance == TIM3)
  {
    HAL_IncTick();
  }

}


void Error_Handler(void)
{

  __disable_irq();
  while (1)
  {
  }
}
#ifdef USE_FULL_ASSERT


void assert_failed(uint8_t *file, uint32_t line)
{

}
#endif

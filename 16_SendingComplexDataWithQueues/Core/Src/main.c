
#include "main.h"
#include "cmsis_os.h"
#include<stdio.h>

UART_HandleTypeDef huart2;

osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);

int __io_putchar(int ch);

void ReceiverTask(void *pvParameters);
void SenderTask(void *pvParameters);


typedef enum
{
	humidity_sensor,
	pressure_sensor
}DataSource_t;

//Define structure type to be passed to the queue
typedef struct
{
	uint8_t ucValue;
	DataSource_t sDataSource;
}Data_t;

//Declare two 'Data_t' variables that will be passed to the queue
static const Data_t xStructsToSend[2] =
{
		{77, humidity_sensor},	//Used by humidity sensor
		{63, pressure_sensor}	//Used by pressure sensor
};

TaskHandle_t hum_task_handle, press_task_handle, receiver_handle;

QueueHandle_t xQueue;


int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();

 //Create queue to hold maximum of three structures
  xQueue = xQueueCreate(3,sizeof(Data_t));

  //Create receiver task with the priority 1
  xTaskCreate(ReceiverTask,
		  	  "Receiver Task",
			  100,
			  NULL,
			  1,
			  &receiver_handle);

  xTaskCreate(SenderTask,
		  	  "Humidity sender task",
			  100,
			  (void*)&(xStructsToSend[0]),
			  2,
			  &hum_task_handle);

  xTaskCreate(SenderTask,
  		  	  "Pressure sender task",
  			  100,
  			  (void*)&(xStructsToSend[1]),
  			  2,
  			  &press_task_handle);

  vTaskStartScheduler();

  while (1)
  {
  }

}

int uart_write(int ch)
{
	while(!(USART2->SR & 0x0080));
	USART2->DR = (ch & 0xFF);

	return ch;
}
int __io_putchar(int ch)
{
	uart_write(ch);
	return ch;
}

void SenderTask(void *pvParameters)
{
	BaseType_t qStatus;

	/*Enter the block state for 200ms for space to become available
	in queue each time the queue is full
	*/
	const TickType_t wait_time = pdMS_TO_TICKS(200);

	while(1)
	{
		qStatus = xQueueSend(xQueue,pvParameters,wait_time);
		if(qStatus != pdPASS)
		{
			//Do something
		}
		for(int i=0;i<100000;i++);
	}
}

void ReceiverTask(void *pvParameters)
{
	Data_t xRecievedStructure;

	BaseType_t qStatus;

	while(1)
	{
		qStatus = xQueueReceive(xQueue, &xRecievedStructure,0);
		if(qStatus==pdPASS)
		{
			if(xRecievedStructure.sDataSource == humidity_sensor)
			{
				printf("Humidity Sensor Value =%d \n\r",xRecievedStructure.ucValue);
			}
			else
			{
				printf("Pressure Sensor Value =%d \n\r",xRecievedStructure.ucValue);
			}
		}
		else
		{
			//Do Something....
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
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
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
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

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
#ifdef USE_FULL_ASSERT
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

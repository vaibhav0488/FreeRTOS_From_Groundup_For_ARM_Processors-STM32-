
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

static int rxInProgress = 0;
static uint16_t rxLen = 0;
static uint8_t * rxBuff = NULL;
static uint16_t rxItr = 0;    //Itr = Iterater


void HandlerTask(void *pvParameters);
int32_t start_rx_interrupt(uint8_t* Buffer, uint_fast16_t Len);

uint8_t btn_state;
uint32_t sensor_value;

#define STACK_SIZE 128  //128*4 = 512 bytes

#define EXPECTED_PKT_LENGTH      5

static QueueHandle_t uart2_BytesReceived;

static SemaphoreHandle_t rxDrone = NULL;

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  USART2_UART_TX_Init();

  //Create Semaphore
  rxDrone = xSemaphoreCreateBinary();

  xTaskCreate(HandlerTask,
  			  "UartPrintTask",
			  STACK_SIZE,
  			  NULL,
  			  tskIDLE_PRIORITY +3,
  			  NULL);

  uart2_BytesReceived = xQueueCreate(10,sizeof(char));

  vTaskStartScheduler();


  while (1)
  {
  }
}

//Set up an interrupt rx for USART2
int32_t start_rx_interrupt(uint8_t* Buffer, uint_fast16_t Len)
{
	if(!rxInProgress && (Buffer !=NULL))
	{
		rxInProgress = 1;
		rxLen = Len;
		rxBuff = Buffer;
		rxItr = 0;
		USART2->CR1 |= 0x0020;  //Enable Rx Interrupt
		NVIC_SetPriority(USART2_IRQn,6);
		NVIC_EnableIRQ(USART2_IRQn);
		return 0;
	}
	return -1;
}

void USART2_IRQHandler(void)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	if(USART2->SR & 0x0020)
	{
		uint8_t tempVal = (uint8_t)USART2->DR;
		if(rxInProgress)
		{
			rxBuff[rxItr++] = tempVal;
			if(rxItr == rxLen)
			{
				rxInProgress = 0;
				xSemaphoreGiveFromISR(rxDrone, &xHigherPriorityTaskWoken);
			}
		}
	}
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}


uint8_t rxData[EXPECTED_PKT_LENGTH];
char rxCode[50] = {0};

void HandlerTask(void *pvParameters)
{
	USART2_UART_RX_Init();

	for(int i=0; i<EXPECTED_PKT_LENGTH; i++)
	{
		rxData[i] = 0;
	}

	const TickType_t timeout = pdMS_TO_TICKS(5000);


	while(1)
	{
		start_rx_interrupt(rxData,EXPECTED_PKT_LENGTH);

		if(xSemaphoreTake(rxDrone, timeout) == pdPASS)
		{
			if(EXPECTED_PKT_LENGTH == rxItr)
			{
				sprintf(rxCode,"Received");
			}
			else
			{
				sprintf(rxCode,"Length mismatch");
			}
		}
		else
			sprintf(rxCode,"Timeout");
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

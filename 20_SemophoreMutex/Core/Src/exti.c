#include<stm32f4xx_hal.h>

void p13_interrupt_init()
{
	/*
	 * Enable GPIOC clock
	 * Enable SYSCFG clock
	 * Configure PC13 for push button interrupt
	 * Clear port selection for EXTI13
	 * Select port C for EXTI13
	 * unmask EXTI13
	 * Select falling edge triggered
	 */
	RCC->AHB1ENR |= (1U<<2);
	RCC->APB2ENR |= 0x4000;
	GPIOC->MODER  &=~0x0C000000;
	SYSCFG->EXTICR[3] &=~0x00F0;
	EXTI->IMR |= 0x2000;
	EXTI->FTSR |= 0x2000;

	NVIC_SetPriority(EXTI15_10_IRQn,6);

	NVIC_EnableIRQ(EXTI15_10_IRQn);

}

void gpio_init()
{
	RCC->AHB1ENR |= (1U<<2);
}

uint8_t read_digital_sensor(void)
{
	if(GPIOC->IDR & 0x2000)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

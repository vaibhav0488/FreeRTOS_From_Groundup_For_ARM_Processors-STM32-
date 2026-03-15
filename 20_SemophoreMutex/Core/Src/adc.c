#include<stm32f4xx_hal.h>


void adc_init()
{
	RCC->AHB1ENR |= (1U<<0);
	RCC->APB2ENR |= (1U<<8);

	GPIOA->MODER |= (1U<<3); //Set PA1 as analog
	GPIOA->MODER |= (1U<<2);

	ADC1->CR2  = 0;  		 //SW trigger
	ADC1->SQR3 = 1;			 //Conversio sequence starts at ch 1
	ADC1->SQR1 = 0;			 //Conversion sequence length 1
	ADC1->CR2 |= 1;			 //enable ADC1
}

uint32_t read_analog_sensor(void)
{
	ADC1->CR2 |= (1U<<30);   //Start conversion

	while(!(ADC1->SR & 2));  //Wait until conversion to complete

	return ADC1->DR;
}

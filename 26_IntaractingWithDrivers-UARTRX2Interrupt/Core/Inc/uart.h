#ifndef __UART_H
#define __UART_H

void USART2_UART_TX_Init(void);
void USART2_UART_RX_Init(void);
char USART2_read(void);
int uart_write(int ch);

#endif

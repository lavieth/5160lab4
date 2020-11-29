
#ifndef  _UART_H
#define  _UART_H



/***** Public Function prototypes ********/

void UART_init (uint8_t volatile * UART_addr, uint16_t Baud_Rate);
int8_t UART_Transmit(uint8_t volatile * UART_addr, int8_t c);
uint8_t UART_Receive(uint8_t volatile * UART_addr);


#endif
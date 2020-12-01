
#include <avr/io.h>
#include "board.h"
#include "UART.h"
#include <avr/pgmspace.h>

/****** Private Constants *******/
#define U2X_bit (1)  // 0 - regular baud rate, 1 - double baud rate

/****** Register Bit Settings *****/
#define RX_enable (1<<4)
#define TX_enable (1<<3)

#define Asynchronous (0<<6)
#define Synchronous (1<<6)
#define No_Parity (0<<4)
#define Even_Parity (2<<4)
#define Odd_Parity (3<<4)
#define One_Stop_Bit (0<<3)
#define Two_Stop_Bits (1<<3)
#define Five_Data_bits (0<<1)
#define Six_Data_bits (1<<1)
#define Seven_Data_bits (2<<1)
#define Eight_Data_bits (3<<1)




void UART_init (uint8_t volatile * UART_addr, uint16_t Baud_Rate)
{
	uint16_t Baud_Rate_Reload;
	Baud_Rate_Reload = (uint16_t)(((F_CPU/F_DIV)/(8UL*(2-U2X_bit)*Baud_Rate))-1);
	
	*(UART_addr+UBBRH) = (Baud_Rate_Reload / 256);
	*(UART_addr+UBBRL) = (Baud_Rate_Reload % 256);
		
	*(UART_addr+UCSRA) = U2X_bit<<1;
	*(UART_addr+UCSRC) = Asynchronous|No_Parity|One_Stop_Bit|Eight_Data_bits;
	*(UART_addr+UCSRB) = RX_enable|TX_enable;
}

int8_t UART_Transmit(uint8_t volatile * UART_addr, int8_t c)
{
	uint8_t status;
	do
	{
		status=*(UART_addr+UCSRA);
	} while ((status & (1 << UDRE))!=(1 << UDRE));
	*(UART_addr+UDR) = c;
	return 0;
}


uint8_t UART_Receive(uint8_t volatile * UART_addr)
{
	uint8_t status;
	do 
	{
		status=*(UART_addr+UCSRA);
	} while ((status & (1 << RXC))!=(1 << RXC));
	return *(UART_addr+UDR);  // return the value in the receive buffer
}


/*
 * Print_Buffer.c
 *
 * Created: 9/4/2020 4:34:38 PM
 *  Author: youngerr
 */ 

#include <avr/io.h>
#include "board.h"
#include "UART.h"
#include <avr/pgmspace.h>

static uint8_t print_buffer[80];

//**** Used to make the global print_buffer available to other source code files *****//
uint8_t * Export_print_buffer(void)
{
	
	return print_buffer;
}

//***** Copies a string from Flash memory to an array (print buffer) in SRAM for easier access  ****//
//***** If num_bytes==0, then the copying continues until a NULL termination is found. ****//
//***** If num_bytes!=0, then the specified number of bytes is copied. ****// 
uint8_t Copy_String_to_Buffer(const int8_t * flash_string, uint8_t num_bytes, uint8_t * buffer_p)
{
	uint8_t index;
	uint8_t value;
	if(num_bytes==0)
	{
		index=0;
		do
		{
			value=pgm_read_byte(&flash_string[index]);
			buffer_p[index]=value;
			index++;
		} while (value!=0);
	}
	else
	{
		for(index=0;index<num_bytes;index++)
		{
			value=pgm_read_byte(&flash_string[index]);
			buffer_p[index]=value;
		}
		index=num_bytes;
	}
	return index;
}

//***** Transmits a string of unsigned bytes stored in an SRAM array (print buffer) using the specified UART.  ****//
//***** If num_bytes==0, then the transmitting continues until a NULL termination is found. ****//
//***** If num_bytes!=0, then the specified number of bytes is transmitted. ****//
void UART_Transmit_String(uint8_t volatile * UART_addr, uint8_t num_bytes, uint8_t * string_name)
{
	int8_t temp8,index;
	if(num_bytes==0)
	{
		temp8=*string_name;
		while (temp8!=0)
		{
			UART_Transmit(UART_addr, temp8);
			string_name++;
			temp8=*string_name;
		}
	}
	else
	{
		for(index=0;index<num_bytes;index++)
		{
			UART_Transmit(UART_addr, string_name[index]);
		}
	}
}


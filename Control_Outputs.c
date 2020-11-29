/*
 * Control_LEDS.c
 *
 * Created: 6/30/2020 11:32:36 AM
 *  Author: youngerr
 */ 

#include <avr/io.h>
#include "board.h"
#include "Control_Outputs.h"

void Output_Init(uint8_t volatile * port_addr, uint8_t pin_mask)
{
	//Set Port Pin to be an output with initial value '1'
	*(port_addr+1)|=(pin_mask);    // Set DDRx to make port pin an output
}


void Output_Set(uint8_t volatile * port_addr, uint8_t pin_mask)
{
	//control = 0, LED off; control = 1, LED on 
		*(port_addr+2)|=(pin_mask);    // Set PORTx PORTx pin to value of '1'
}

void Output_Clear(uint8_t volatile * port_addr, uint8_t pin_mask)
{
	//control = 0, LED off; control = 1, LED on
		*(port_addr+2)&=~(pin_mask);    // Clear PORTx pin to value of '0'
}

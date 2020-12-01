#ifndef _print_bytes_H
#define _print_bytes_H

#include "board.h"



// ------ Public function prototypes -------------------------------

void print_memory(uint8_t volatile * UART_addr, uint16_t number_of_bytes, uint8_t * array_in);


#endif
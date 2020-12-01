/*
 * Print_Buffer.h
 *
 * Created: 9/4/2020 4:36:37 PM
 *  Author: youngerr
 */ 


#ifndef PRINT_BUFFER_H_
#define PRINT_BUFFER_H_

int8_t * Export_print_buffer(void);
uint8_t Copy_String_to_Buffer(const int8_t * flash_string, uint8_t num_bytes, int8_t * buffer_p);
void UART_Transmit_String(uint8_t volatile * UART_addr, uint8_t num_bytes,int8_t * string_name);



#endif /* PRINT_BUFFER_H_ */
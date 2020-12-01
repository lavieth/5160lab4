/*
 * I2C.c
 *
 * Created: 7/2/2020 1:57:23 PM
 * Author : youngerr
 */ 

#include <avr/io.h>
#include <stdio.h>
#include "Control_Outputs.h"
#include "UART.h"
#include "UART_Print.h"
#include "SPI.h"
#include "SDCard.h"
#include "Long_Serial_In.h"
#include "board.h"
#include "print_memory.h"
#include "Read_sector.h"
#include "Directory_Functions_globals.h"
#include <avr/pgmspace.h>

const char LSI_Prompt[16] PROGMEM = {"Enter block #: "};
	
	uint8_t buffer1[512];
	uint8_t buffer2[512];

int main(void)
{
    /* Replace with your application code */
	int8_t *prnt_bffr;
	uint32_t temp32;

	
	prnt_bffr=Export_print_buffer();
	Output_Set(&PA,(LED3|OLED_C_D));
	Output_Init(&PA,(LED3|OLED_C_D));
	Output_Set(&PB,(SD_CS|LED1|OLED_CS));
	Output_Init(&PB,(SD_CS|LED1|OLED_CS));
	Output_Set(&PC,(USER_LED));
	Output_Init(&PC,(USER_LED));
	Output_Set(&PE,(LED2));
	Output_Init(&PE,(LED2));
	
	SPI_Master_Init(&SPI0,400000);
	//OLED_Init();
	UART_init(&UART1,9600);
	SD_Card_Init();
	SPI_Master_Init(&SPI0,2000000);
    while (1) 
    {
		 Copy_String_to_Buffer(LSI_Prompt,0,prnt_bffr);
		 UART_Transmit_String(&UART1,0,prnt_bffr);
		 temp32=long_serial_input(&UART1);
		 Output_Clear(&PB,LED1);
		 /*Output_Clear(&PB,SD_CS);  // Clear nCS = 0
		 Send_Command(17,temp32);
		 Read_Block(512,buffer1);
		 Output_Set(&PB,LED1);
		 Output_Set(&PB,SD_CS);  // Set nCS = 1*/
		Mount_Drive(buffer1);

	
		
		 	print_memory(&UART1, 512, buffer1);

    }
}


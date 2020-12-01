#ifndef _SDCard_H
#define _SDCard_H

#include "board.h"
// ------ Public Constant definitions -----------------------------

// error values
#define no_errors (0)
#define timeout_error (0x81)
#define illegal_cmnd (0x82)
#define response_error (0x83)
#define data_error (0x84)
#define voltage_error (0x85)
#define card_inactive (0x86)
#define SPI_error (0x87)
#define BPB_not_found (1)

// SD Card types
#define unknown (0xFF)
#define Ver2 (0x02)
#define Standard_Capacity (9)  // used to shift address for block to byte address conversion
#define High_Capacity (0)      // no shift when no conversion is required

// SD Card Commands 
#define CMD0 0
#define CMD8 8
#define CMD16 16
#define CMD17 17
#define CMD55 55
#define CMD58 58
#define ACMD41 41


// ------ Public function prototypes -------------------------------
uint8_t Send_Command(uint8_t cmnd, uint32_t argum);

uint8_t Receive_Response(uint8_t num, uint8_t * valout);

uint8_t Read_Block(uint16_t num, uint8_t * array_out);

uint8_t SD_Card_Init(void);

uint8_t Return_SD_Card_Type(void);

void Print_Error(uint8_t error);


#endif
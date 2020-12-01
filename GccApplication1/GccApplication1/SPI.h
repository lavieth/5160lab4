#ifndef _SPI_H
#define _SPI_H

#include "board.h"

//------- Public Constant definitions --------------------------------
// error values
#define init_okay (0)
#define no_errors (0)
#define illegal_clockrate (0x0F)


// ------ Public function prototypes -------------------------------
uint8_t SPI_Master_Init(uint8_t volatile *SPI_addr, uint32_t clock_rate);
uint8_t SPI_Transfer(uint8_t volatile *SPI_addr, uint8_t data_input);

#endif
#ifndef SD_READ_SECTOR_H
#define SD_READ_SECTOR_H
#include "stdint.h"

uint8_t Read_Sector(uint32_t sector_number, uint16_t sector_size, uint8_t * array_for_data);




#endif

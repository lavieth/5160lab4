#ifndef READ_VALUES_H
#define READ_VALUES_H
#include "stdint.h"
uint8_t read_value_8 (uint16_t offset, uint8_t* array_name); 
uint16_t read_value_16 (uint16_t offset, uint8_t* array_name);
uint32_t read_value_32 (uint16_t offset, uint8_t* array_name);





#endif

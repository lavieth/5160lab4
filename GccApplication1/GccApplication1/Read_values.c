#include "Read_values.h"

//do we need the switch for Big to little endian function? part b) of 3 seems to ask for it


uint8_t read_value_8 (uint16_t offset, uint8_t * array_name)
{
  uint8_t temp8 = *(array_name+offset);
  return temp8;
}
uint16_t read_value_16 (uint16_t offset, uint8_t * array_name)
{
  uint16_t temp16 = *(array_name+offset);

  return temp16;
}


uint32_t read_value_32 (uint16_t offset, uint8_t * array_name)
{
  uint32_t temp32 = *(array_name+offset);
  return temp32;

}

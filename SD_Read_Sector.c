#include "SD_Read_Sector.h"
#include "SDCard.h"
#include "Control_Outputs.h"

uint8_t Read_Sector(uint32_t sector_number, uint16_t sector_size, uint8_t * array_for_data)
{
  uint8_t SDtype, error_flag = no_errors;
  //if HC=0 no change to sector number
  //SC=9 multiplies sector num by 512 to convert to byte addr.
  SDtype=Return_SD_Card_Type;
  Output_Clear(&PB, SDCS);
  error_flag=Send_Command(17, (sector_number<<SDtype));
  if(error_flag==no_errors)
      error_flag=Read_Block(sector_size,array_for_data);
  Output_Set(&PB,SDCS);
  if(error_flag!=no_errors)
      error_flag=disk_error; 
  
  return error_flag;
}

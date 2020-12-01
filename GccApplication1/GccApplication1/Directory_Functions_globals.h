#ifndef _Directory_Func_H
#define _Directory_Func_H

#include "board.h"

//------- Public Constants  -----------------------------------------
#define FAT32 (4)
#define FAT16 (2)
#define FAT32_shift (2)
#define FAT16_shift (1)
#define Disk_Error (0xF0)
#define No_Disk_Error (0)
#define more_entries (0x8000)   
#define no_entry_found (0x80000000)  // msb set to indicate error
#define directory_bit  (0x10000000)  // lsb of first nibble (bit28)

// ------ Public function prototypes -------------------------------

uint16_t Print_Directory(uint32_t Sector_num, uint8_t *array_in);

uint32_t Read_Dir_Entry(uint32_t Sector_num, uint16_t Entry, uint8_t *array_in);

// ------ Function prototypes needed (These can be defined in a separate file) -------------

//uint8 read_value_8(uint16 offset, uint8 * array_name);

//uint16 read_value_16(uint16 offset, uint8 * array_name);

//uint32 read_value_32(uint16 offset, uint8 * array_name);

uint8_t Mount_Drive(uint8_t * array_name);

//uint32 First_Sector (uint32 Cluster_num);

//uint32 Find_Next_Clus(uint32 Cluster_num, uint8 xdata * array_name);

//uint8 Open_File(uint32 Cluster, uint8 xdata * array_in);

#endif
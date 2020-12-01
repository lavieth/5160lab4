#include <stdio.h>
#include "board.h"
#include "UART.h"
#include "UART_Print.h"
#include "SPI.h"
#include "SDCard.h"
#include "Directory_Functions_globals.h"
#include "print_memory.h"
//#include "File_System_globals.h"
#include "Read_Sector.h"
#include "Read_values.h"

/******* Private Constants *************/
#define CR (0x0D)
#define LF (0x0A)

uint32_t FirstDataSec_g, StartofFAT_g, FirstRootDirSec_g, RootDirSecs_g;
uint16_t BytesPerSec_g;
uint8_t SecPerClus_g, FATtype_g, BytesPerSecShift_g,FATshift_g;









/***********************************************************************
DESC: Prints all short file name entries for a given directory 
INPUT: Starting Sector of the directory and the pointer to a 
block of memory in xdata that can be used to read blocks from the SD card
RETURNS: uint16_t number of entries found in the directory
CAUTION: Supports FAT16, SD_shift must be set before using this function
************************************************************************/



uint16_t  Print_Directory(uint32_t Sector_num, uint8_t *array_in)
{ 
   uint32_t Sector, max_sectors;
   uint16_t i, entries;
   uint8_t temp8, j, attr, out_val, error_flag;
   uint8_t *values;
   uint8_t *prnt_bffr;

   prnt_bffr=Export_print_buffer();
   values=array_in;
   entries=0;
   i=0;
   if (Sector_num<FirstDataSec_g)  // included for FAT16 compatibility
   { 
      max_sectors=RootDirSecs_g;   // maximum sectors in a FAT16 root directory
   }
   else
   {
      max_sectors=SecPerClus_g;
   }
   Sector=Sector_num;
   error_flag=Read_Sector(Sector, BytesPerSec_g, values);
   if(error_flag==no_errors)
   {
     do
     {
 
	    temp8=read_value_8(0+i,values);  // read first byte to see if empty
        if((temp8!=0xE5)&&(temp8!=0x00))
	    {  
	       attr=read_value_8(0x0b+i,values);
		   if((attr&0x0E)==0)   // if hidden, system or Vol_ID bit is set do not print
		   {
		      entries++;
			  sprintf(prnt_bffr,"%5d. ",entries);  // print entry number with a fixed width specifier
			  UART_Transmit_String(&UART1,0,prnt_bffr);
		      for(j=0;j<8;j++)
			  {
			     out_val=read_value_8(i+j,values);   // print the 8 byte name
			     UART_Transmit(&UART1,out_val);
			  }
              if((attr&0x10)==0x10)  // indicates directory
			  {
			     for(j=8;j<11;j++)
			     {
			        out_val=read_value_8(i+j,values);
			        UART_Transmit(&UART1,out_val);
			     }
			     sprintf(prnt_bffr,"[DIR]\n\r");
				 UART_Transmit_String(&UART1,0,prnt_bffr);
			  }
			  else       // print a period and the three byte extension for a file
			  {
			     UART_Transmit(&UART1,0x2E);       
			     for(j=8;j<11;j++)
			     {
			        out_val=read_value_8(i+j,values);
			        UART_Transmit(&UART1,out_val);
			     }
			     UART_Transmit(&UART1,CR);
                 UART_Transmit(&UART1,LF);
			  }
		    }

		}
		i=i+32;  // next entry

		if(i>510)
		{
		  Sector++;
          if((Sector-Sector_num)<max_sectors)
		  {
              error_flag=Read_Sector(Sector, BytesPerSec_g, values);
			  if(error_flag!=no_errors)
			    {
			      entries=0;   // no entries found indicates disk read error
				  temp8=0;     // forces a function exit
			    }
			    i=0;
		  }
		  else
		  {
			  entries=entries|more_entries;  // set msb to indicate more entries in another cluster
			  temp8=0;                       // forces a function exit
		  }
		}
       
	  }while(temp8!=0);
	}
	else
	{
	   entries=0;    // no entries found indicates disk read error
	}
    return entries;
 }


/***********************************************************************
DESC: Uses the same method as Print_Directory to locate short file names,
      but locates a specified entry and returns and cluster  
INPUT: Starting Sector of the directory, an entry number and a pointer to a 
block of memory in SRAM that can be used to read blocks from the SD card
RETURNS: uint32_t with cluster in lower 28 bits.  Bit 28 set if this is 
         a directory entry, clear for a file.  Bit 31 set for error.
CAUTION: 
************************************************************************/

uint32_t Read_Dir_Entry(uint32_t Sector_num, uint16_t Entry, uint8_t *array_in)
{ 
   uint32_t Sector, max_sectors, return_clus;
   uint16_t i, entries;
   uint8_t temp8, attr, error_flag;
   uint8_t * values;

   values=array_in;
   entries=0;
   i=0;
   return_clus=0;
   if (Sector_num<FirstDataSec_g)  // included for FAT16 compatibility
   { 
      max_sectors=RootDirSecs_g;   // maximum sectors in a FAT16 root directory
   }
   else
   {
      max_sectors=SecPerClus_g;
   }
   Sector=Sector_num;
   error_flag=Read_Sector(Sector, BytesPerSec_g, values);
   if(error_flag==no_errors)
   {
     do
     {
        temp8=read_value_8(0+i,values);  // read first byte to see if empty
        if((temp8!=0xE5)&&(temp8!=0x00))
	    {  
	       attr=read_value_8(0x0b+i,values);
		   if((attr&0x0E)==0)    // if hidden do not print
		   {
		      entries++;
              if(entries==Entry)
              {
			    if(FATtype_g==FAT32)
                {
                   return_clus=read_value_8(21+i,values);
				   return_clus&=0x0F;            // makes sure upper four bits are clear
				   return_clus=return_clus<<8;
                   return_clus|=read_value_8(20+i,values);
                   return_clus=return_clus<<8;
                }
                return_clus|=read_value_8(27+i,values);
			    return_clus=return_clus<<8;
                return_clus|=read_value_8(26+i,values);
			    attr=read_value_8(0x0b+i,values);
			    if(attr&0x10) return_clus|=directory_bit;
                temp8=0;    // forces a function exit
              }
              
		   }
        }
		i=i+32;  // next entry
		if(i>510)
		{
		   Sector++;
		   if((Sector-Sector_num)<max_sectors)
		   {
              error_flag=Read_Sector(Sector, BytesPerSec_g, values);
			  if(error_flag!=no_errors)
			  {
			     return_clus=no_entry_found;
                 temp8=0; 
			  }
			  i=0;
		   }
		   else
		   {
			  temp8=0;                       // forces a function exit
		   }
		}
        
	 }while(temp8!=0);
   }
   else
   {
	 return_clus=no_entry_found;
   }
   if(return_clus==0) return_clus=no_entry_found;
   return return_clus;
}


uint8_t Mount_Drive(uint8_t * array_name)
{
	uint8_t error_flag = no_errors;
	Read_Sector((uint32_t)(8192), 512, array_name);
	
	//BPB has been read
	if((read_value_8(0, array_name) == 0xEB) || (read_value_8(0, array_name) == 0xE9))
	{
		//If FAT32, RootEntCount and RootDirSecs will both be 0
		uint16_t RootEntCount = read_value_16(0x11, array_name);
		BytesPerSec_g = read_value_16(0x0B, array_name);
		RootDirSecs_g = (((RootEntCount * 32) + (BytesPerSec_g - 1)) / BytesPerSec_g);
		
		uint16_t FATSz16 = read_value_16(0x16, array_name);
		uint32_t FATSz32 = read_value_32(0x24, array_name);
		uint16_t TotSec16 = read_value_16(0x13, array_name);
		uint32_t TotSec32 = read_value_32(0x20, array_name);
		uint16_t ResvdSecCount = read_value_16(0x0E, array_name);
		uint8_t NumFATs = read_value_8(0x10, array_name);
		uint32_t HiddenSec = read_value_32(0x1C, array_name);
		uint32_t RootClus = read_value_32(0x2C, array_name);
		SecPerClus_g = read_value_8(0x0D, array_name);
		uint32_t FATSz;
		uint32_t TotSec;
		uint32_t DataSec;
		uint32_t CountofClusters;
		uint8_t FATOffset;
		uint8_t N = 0;
		
		if(FATSz16 != 0)
		FATSz = FATSz16;
		
		else
		FATSz = FATSz32;

		if(TotSec16 != 0)
		TotSec = TotSec16;
		
		else
		TotSec = TotSec32;
		
		DataSec = TotSec - (ResvdSecCount + (NumFATs * FATSz) + RootDirSecs_g);
		
		CountofClusters = DataSec / SecPerClus_g;
		
		if(CountofClusters < 65525)
		FATtype_g = FAT16;
		
		else
		FATtype_g = FAT32;
		
		//Set N = 0 to determine first sector of FAT
		if(FATtype_g == FAT16)
		FATOffset = N * 2;
		
		else if (FATtype_g == FAT32)
		FATOffset = N * 4;
		
		StartofFAT_g = ResvdSecCount + HiddenSec;
		
		FirstDataSec_g = ResvdSecCount + (NumFATs * FATSz) + RootDirSecs_g + HiddenSec;
		
		if(FATtype_g == FAT16)
		FirstRootDirSec_g = ResvdSecCount + (NumFATs * FATSz) + HiddenSec;
		
		else if (FATtype_g == FAT32)
		FirstRootDirSec_g = ((RootClus -2) * SecPerClus_g) + FirstDataSec_g;
	}
	
	else 
	{
		Read_Sector(read_value_32(0x01C6, array_name), 512, array_name);
		
		if(read_value_8(0, array_name) == 0xEB || read_value_8(0, array_name) == 0xE9)
		{
				//If FAT32, RootEntCount and RootDirSecs will both be 0
				uint16_t RootEntCount = read_value_16(0x11, array_name);
				BytesPerSec_g = read_value_16(0x0B, array_name);
				RootDirSecs_g = (((RootEntCount * 32) + (BytesPerSec_g - 1)) / BytesPerSec_g);
				
				uint16_t FATSz16 = read_value_16(0x16, array_name);
				uint32_t FATSz32 = read_value_32(0x24, array_name);
				uint16_t TotSec16 = read_value_16(0x13, array_name);
				uint32_t TotSec32 = read_value_32(0x20, array_name);
				uint16_t ResvdSecCount = read_value_16(0x0E, array_name);
				uint8_t NumFATs = read_value_8(0x10, array_name);
				uint32_t HiddenSec = read_value_32(0x1C, array_name);
				uint32_t RootClus = read_value_32(0x2C, array_name);
				SecPerClus_g = read_value_8(0x0D, array_name);
				uint32_t FATSz;
				uint32_t TotSec;
				uint32_t DataSec;
				uint32_t CountofClusters;
				uint8_t FATOffset;
				uint8_t N = 0;
				
				if(FATSz16 != 0)
				FATSz = FATSz16;
				
				else
				FATSz = FATSz32;

				if(TotSec16 != 0)
				TotSec = TotSec16;
				
				else
				TotSec = TotSec32;
				
				DataSec = TotSec - (ResvdSecCount + (NumFATs * FATSz) + RootDirSecs_g);
				
				CountofClusters = DataSec / SecPerClus_g;
				
				if(CountofClusters < 65525)
				FATtype_g = FAT16;
				
				else
				FATtype_g = FAT32;
				
				//Set N = 0 to determine first sector of FAT
				if(FATtype_g == FAT16)
				FATOffset = N * 2;
				
				else if (FATtype_g == FAT32)
				FATOffset = N * 4;
				
				StartofFAT_g = ResvdSecCount + HiddenSec;
				
				FirstDataSec_g = ResvdSecCount + (NumFATs * FATSz) + RootDirSecs_g + HiddenSec;
				
				if(FATtype_g == FAT16)
				FirstRootDirSec_g = ResvdSecCount + (NumFATs * FATSz) + HiddenSec;
				
				else if (FATtype_g == FAT32)
				FirstRootDirSec_g = ((RootClus -2) * SecPerClus_g) + FirstDataSec_g;
		}
		
		//couldn't find BPB
		else
		{
			error_flag = BPB_not_found;
		}
	}

	return error_flag;	
}





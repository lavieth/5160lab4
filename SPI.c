
#include <avr/io.h>
#include "board.h"
#include "SPI.h"
#include "Control_Outputs.h"

/***** Private Constants ******/
#define Clock_Polarity (0)
#define Clock_Phase (0)

/***********************************************************************
DESC:    Sets up the SPI to master mode with the clock as close
         to the input parameter as possible.
         clock=32-bit 
RETURNS: Error Flag
CAUTION: Sets the CPHA to 0 and CPOL to 0
         Disables SS and sets master mode 
************************************************************************/

uint8_t SPI_Master_Init(uint8_t volatile *SPI_addr, uint32_t clock_rate)
{
  uint8_t divider,return_val,SPR_val;
  // Enable MOSI and SCK outputs
  if(SPI_addr==&SPI0)
  {
     Output_Set(&PB,MOSI);
     Output_Init(&PB,MOSI);
	 if(Clock_Polarity==0)
	 {
	   Output_Clear(&PB,SCK);
	   Output_Init(&PB,SCK);
	 }
	 else
	 {
	   Output_Set(&PB,SCK);
	   Output_Init(&PB,SCK);
	 }
  }
  else if(SPI_addr==&SPI1)
  {
     Output_Set(&PE,(1<<3));
     Output_Init(&PE,(1<<3));
	 if(Clock_Polarity==0)
	 {
	   Output_Clear(&PD,(1<<7));
	   Output_Init(&PD,(1<<7));
	 }
	 else
	 {
	   Output_Set(&PD,(1<<7));
	   Output_Init(&PD,(1<<7));
	 }
  }


  return_val=no_errors;
  divider=(uint8_t)((F_CPU/F_DIV)/clock_rate);
  if(divider<2)
  {
     SPR_val=0;
	 *(SPI_addr+SPCR)=((1<<SPE)|(1<<MSTR)|(Clock_Polarity<<3)|(Clock_Phase<<2)|SPR_val);
	 *(SPI_addr+SPSR)=1;
  }
  else if((divider>=2)&&(divider<4))
  {
     SPR_val=0;
	 *(SPI_addr+SPCR)=((1<<SPE)|(1<<MSTR)|(Clock_Polarity<<3)|(Clock_Phase<<2)|SPR_val);
	 *(SPI_addr+SPSR)=0;
  }
  else if((divider>=4)&&(divider<8))
  {
     SPR_val=1;
	 *(SPI_addr+SPCR)=((1<<SPE)|(1<<MSTR)|(Clock_Polarity<<3)|(Clock_Phase<<2)|SPR_val);
	 *(SPI_addr+SPSR)=1;
  }
  else if((divider>=8)&&(divider<16))
  {
     SPR_val=1;
	 *(SPI_addr+SPCR)=((1<<SPE)|(1<<MSTR)|(Clock_Polarity<<3)|(Clock_Phase<<2)|SPR_val);
	 *(SPI_addr+SPSR)=0;
  } 
  else if((divider>=16)&&(divider<32))
  {
     SPR_val=2;
	 *(SPI_addr+SPCR)=((1<<SPE)|(1<<MSTR)|(Clock_Polarity<<3)|(Clock_Phase<<2)|SPR_val);
	 *(SPI_addr+SPSR)=1;
  }
  else if((divider>=32)&&(divider<64))
  {
     SPR_val=2;
	 *(SPI_addr+SPCR)=((1<<SPE)|(1<<MSTR)|(Clock_Polarity<<3)|(Clock_Phase<<2)|SPR_val);
	 *(SPI_addr+SPSR)=0;
  }
  else if((divider>=64)&&(divider<128))
  {
     SPR_val=3;
	 *(SPI_addr+SPCR)=((1<<SPE)|(1<<MSTR)|(Clock_Polarity<<3)|(Clock_Phase<<2)|SPR_val);
	 *(SPI_addr+SPSR)=0;
  }
  else  // if the SPI clock rate is too slow, a divider cannot be found
  {
    return_val=illegal_clockrate;
  }
  return return_val;
}

/***********************************************************************
DESC:    Sends one byte using the SPI port and returns the received byte
          
RETURNS: SPI Error Flags | received byte
         or a timeout error
CAUTION: Waits for the SPI transfer to be complete
************************************************************************/


uint8_t SPI_Transfer(uint8_t volatile *SPI_addr, uint8_t data_input)
{
   uint8_t test;
   *(SPI_addr+SPDR)=data_input;
   do
   {
      test=*(SPI_addr+SPSR);
   }while((test&0x80)==0);
   
   return *(SPI_addr+SPDR);
}
 


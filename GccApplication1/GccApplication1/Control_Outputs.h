/*
 * Control_Outputs.h
 *
 * Created: 6/30/2020 11:42:34 AM
 *  Author: youngerr
 */ 


#ifndef CONTROL_OUTPUTS_H_
#define CONTROL_OUTPUTS_H_

void Output_Init(uint8_t volatile * port_addr, uint8_t pin_mask);
void Output_Set(uint8_t volatile * port_addr, uint8_t pin_mask);
void Output_Clear(uint8_t volatile * port_addr, uint8_t pin_mask);



#endif /* CONTROL_OUTPUTS_H_ */
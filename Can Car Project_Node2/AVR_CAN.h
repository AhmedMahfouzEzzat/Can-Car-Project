/*
 * AVR_CAN.h
 *
 * Created: 03/04/2019 03:27:54 AM
 *  Author: Ahmed
 */ 


#ifndef AVR_CAN_H_
#define AVR_CAN_H_

#include "mcp2515.h"
#include "AVR_SPI.h"


unsigned char * msgReceived = 0;
unsigned char rbuffer[14];  /* 2 RX buffers, each have 14B */
#define changeBits(reg_data , bit_mask , new_value) writeRegister( reg_data, ((readRegister(reg_data) & (~ bit_mask))| (new_value))) 
		
#define getMode (readRegister(CANSTAT) >> 5)
#define setMode(mode) { changeBits(CANCTRL, (7 << REQOP0), \
 (mode << REQOP0)); while(getMode != mode); }
 
#define setRollover(v) changeBits(RXB0CTRL, 1 << BUKT, v << BUKT)
#define setMask(n, c, e) setAcceptanceCriteria(RXMnSIDH(n), c, e)
#define setFilter(n, c, e) setAcceptanceCriteria(RXFnSIDH(n), c, e)

#define getData(n) msgReceived[6+i]
#define getId (unsigned short)((msgReceived[1]<<3)|(msgReceived[2]>>5))
#define getLength msgReceived[5] >> 4

unsigned char readRegister(unsigned char address);

void writeRegister(unsigned char address, unsigned char value);

void resetMCP2515() ;

unsigned char setBitTiming(unsigned char rCNF1,
							 unsigned char rCNF2,
							 unsigned char rCNF3);

void setAcceptanceCriteria (unsigned char address,
							 unsigned long criterion,
							 unsigned char is_ext) ;

void initMCP2515(void);

void sendCANmsg(unsigned char bi,
				 unsigned long id,
				 unsigned char * data,
				 unsigned char prop) ;
				 
void interruptMCP2515(void) ;

#endif /* AVR_CAN_H_ */
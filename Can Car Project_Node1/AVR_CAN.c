/*
 * AVR_CAN.c
 *
 * Created: 03/04/2019 03:27:11 AM
 *  Author: Ahmed
 */ 
#include "AVR_CAN.h"


unsigned char * msgReceived = 0;
unsigned char rbuffer[14];  /* 2 RX buffers, each have 14B */
/**
 * Read value of the register on selected address inside the
 * MCP2515. Works for every register.
 *
 * \see MCP2515 datasheet, chapter 11 - register description
 * \see MCP2515 datasheet, chapter 12 - read instruction
 * \param address Register address
 */
unsigned char readRegister(unsigned char address)
{
	 /* Send read instruction, address, and receive result */
	 spiMasterChipSelect(1);
	 spiMasterTRANSMIT(READ_INSTRUCTION);
	 spiMasterTRANSMIT(address);
	unsigned char buffer = spiMasterTRANSMIT(0);
	 spiMasterChipSelect(0);
	 return buffer;
}
/**
 * Change value of the register on selected address inside the
 * MCP2515. Works for every register.
 *
 * \see MCP2515 datasheet, chapter 11 - register description
 * \see MCP2515 datasheet, chapter 12 - write instruction
 * \param address Register address
 * \param value New value of the register
 */
void writeRegister(unsigned char address, unsigned char value)
{
	/* Send write instruction, address, and data */
	 spiMasterChipSelect(1);
	 spiMasterTRANSMIT(WRITE_INSTRUCTION);
	 spiMasterTRANSMIT(address);
	 spiMasterTRANSMIT(value);
	 spiMasterChipSelect(0);
}
/**
 * Send reset instruction to the MCP2515. Device should
 * reinitialize yourself and go to the configuration mode
 */
void resetMCP2515()
{
	 /* Send reset instruction */
	 spiMasterChipSelect(1);
	 spiMasterTRANSMIT(RESET_INSTRUCTION);
	 spiMasterChipSelect(0);
}

unsigned char setBitTiming(unsigned char rCNF1, unsigned char rCNF2, unsigned char rCNF3)
{
	 if(getMode == CONFIGURATION_MODE) { 
		 writeRegister(CNF1, rCNF1);
		 writeRegister(CNF2, rCNF2);
		 changeBits(CNF3, 0x07, rCNF3);
		 return 1;
	 }
	 return 0;
}

/** Set up acceptance filters/masks
 * \param address starting address of 4 registers to setup. It can be mask
 * or filter, doesn't matter
 * \param criterion message identifier criterion to be set
 * \param is_ext 1 if message is extended, otherwise 0 */
void setAcceptanceCriteria (unsigned char address,unsigned long criterion,unsigned char is_ext)
{
	 /* Initialize reading of the receive buffer */
	 spiMasterChipSelect(1);
	 /* Send header and address */
	 spiMasterTRANSMIT(WRITE_INSTRUCTION);
	 spiMasterTRANSMIT(address);
	 /* Setup standard or extended identifier */
	 if(is_ext) {
	 spiMasterTRANSMIT((unsigned char)(criterion>>3));
	 spiMasterTRANSMIT((unsigned char)(criterion<<5)|(1<<EXIDE)|((unsigned char)(criterion>>27)));
	 spiMasterTRANSMIT((unsigned char)(criterion>>19));
	 spiMasterTRANSMIT((unsigned char)(criterion>>11));
	 } else {
	 spiMasterTRANSMIT((unsigned char)(criterion >> 3));
	 spiMasterTRANSMIT((unsigned char)(criterion << 5));
	 }
	 /* Release the bus */
	 spiMasterChipSelect(0);
}

/* Configuration routine */
void initMCP2515(void)
{
	/* Initialize SPI as a master device, on frequency < 10Mhz */
	 spiMasterINIT();
	/* Initialize external interrupt service on this device */
	 extInterruptINIT(interruptMCP2515);
	/* Send reset instruction */
	 resetMCP2515();
	/* Set configuration mode */
	 setMode(CONFIGURATION_MODE);
	/* Set bit timing , masks and rollover mode*/
	 setBitTiming(0x04, 0xD2, 0x42);
	 setMask(RXM0, 0x00000000, 1);
	 setMask(RXM1, 0x00000000, 1);
	 setRollover(1);
	/* Get into normal mode and setup communication */
	 setMode(NORMAL_MODE)
}

/** Send a CAN message
 * \param bi transmit buffer index
 * \param id message identifier
 * \param data pointer to data to be stored
 * \param prop message properties, the octet has following structure:
 * - bits 7:6 - message priority (higher the better)
 * - bit 5 - if set, message is remote request (RTR)
 * - bit 4 - if set, message is considered to have ext. id.
 * - bits 3:0 - message length (0 to 8 bytes) */
void sendCANmsg(unsigned char bi,unsigned long id,unsigned char * data,unsigned char prop)
{
	/* Initialize reading of the receive buffer */
	 spiMasterChipSelect(1);
	/* Send header and address */
	 spiMasterTRANSMIT(WRITE_INSTRUCTION);
	 spiMasterTRANSMIT(TXBnCTRL(bi));
	 /* Setup message priority */
	 spiMasterTRANSMIT(prop >> 6);
	/* Setup standard or extended identifier */
	 if(prop & 0x10) {
	 spiMasterTRANSMIT((unsigned char)(id>>3));
	 spiMasterTRANSMIT((unsigned char)(id<<5)|(1<<EXIDE)|((unsigned char)(id>>27)));
	 spiMasterTRANSMIT((unsigned char)(id>>19));
	 spiMasterTRANSMIT((unsigned char)(id>>11));
	 } else {
	 spiMasterTRANSMIT((unsigned char)(id>>3));
	 spiMasterTRANSMIT((unsigned char)(id<<5));
	 }
	 /* Setup message length and RTR bit */
	 spiMasterTRANSMIT((prop & 0x0F) | ((prop & 0x20) ? (1 << RTR) : 0));
	/* Store the message into the buffer */
	 for(unsigned char i = 0; i < (prop & 0x0F); i++)
	 spiMasterTRANSMIT(data[i]);
	 /* Send request to send */
	 sendRTS(bi);
	/* Release the bus */
	 spiMasterChipSelect(0);
	
}

void interruptMCP2515(void)
{
	 
	 /* Copy the message from the device and release buffer */
	 spiMasterTRANSMIT(READ_INSTRUCTION);
	 spiMasterTRANSMIT(RXBnCTRL(0));
	 /* Make the local copy */
	 for(unsigned char i=0; i < 14; i++)
	 rbuffer[i] = spiMasterTRANSMIT(0);
	 msgReceived  = rbuffer;
}
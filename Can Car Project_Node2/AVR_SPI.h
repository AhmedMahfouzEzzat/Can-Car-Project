/*
 * AVR_SPI.h
 *
 * Created: 03/04/2019 12:14:56 AM
 *  Author: Ahmed
 */ 


#ifndef AVR_SPI_H_
#define AVR_SPI_H_

#define DDR_SPI DDRB /* Data dir. register for port with SPI */
#define PORT_SPI PORTB /* Port with SPI */
#define PIN_MOSI PB3 /* MOSI pin on the PORTB_SPI */
#define PIN_MISO PB4 /* MISO pin on the PORTB_SPI */
#define PIN_SCK PB5 /* SCK pin on the PORTB_SPI */
#define PIN_SS PB2 /* SS pin on the PORTB_SPI */

void spiMasterINIT() ;
unsigned char spiMasterTRANSMIT(unsigned char data) ;
void spiMasterChipSelect(unsigned char state) ;

/* Pointer to function which handle change on INT pin handler */
void (*int_handler)(void);

void extInterruptINIT(void (*handler)(void));


#endif /* AVR_SPI_H_ */
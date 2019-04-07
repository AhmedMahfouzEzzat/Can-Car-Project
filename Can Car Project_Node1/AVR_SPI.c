/*
 * AVR_SPI.c
 *
 * Created: 03/04/2019 12:16:12 AM
 *  Author: Ahmed
 */ 
#include "avr/io.h"
#include "avr/interrupt.h"
#include "AVR_SPI.h"
/** \brief Initialization of the SPI interface on the MCU
 *
 * Initialization of the SPI hardware interface - configure this
 * device as master, set mode 0,0 and the communication speed (there
 * is limitation - 10Mhz, nominal speed should be >8Mhz, for this
 * purpose.
 *
 * \warning This is platform-dependent method!
 *
 */
void spiMasterINIT()
{
	/* Set MOSI and SCK output, all others input */
	 DDR_SPI = (1<<PIN_MOSI)|(1<<PIN_SCK)|(1<<PIN_SS);
	 PORT_SPI |= (1 << PIN_SS);
	/* Enable SPI, Master, set clock rate fck/4, mode 0,0 */
	 SPCR = (1<<SPE) | (1<<MSTR);
	 SPSR = (1<<SPI2X);
}
/* \brief Transmiting databytes via the SPI
 *
 * This function is transmitting data via the SPI interface. Input
 * parameter is uns. char array. Data are transmited from the zero
 * index
 *
 * \warning This is platform-dependent method!
 * \param data[] Source data array
 * \param length Array length
 *
 */
unsigned char spiMasterTRANSMIT(unsigned char data)
{
	/* Start transmission */
	 SPDR = data;
	/* Wait for transmission complete */
	 while(!(SPSR & (1<<SPIF)));
	/* SPDR must be stored as quickly
	 as possible (ref. ATMegaX ds) */
	 return SPDR;
}
/** \brief Settings of the CS pin
 *
 * This function is used for setting of the CS pin. CS signal
 * is inverted, so input 1 (true) means zero on the output.
 * Otherwise is analogically the same.
 *
 * \warning This is platform-dependent method!
 * \param state Wished state
 */
void spiMasterChipSelect(unsigned char state)
{
	/* What the user wants? (remember that the CS signal is inverted) */
	 if(!state) {
	 /* Upper the CS pin */
	 PORT_SPI |= (1<<PIN_SS);
	 DDR_SPI |= (1<<PIN_SS);
	 } else {
	 /* Lower the CS pin */
	 PORT_SPI &= ~(1<<PIN_SS);
	 DDR_SPI |= (1<<PIN_SS);
	 }
}

/** Initialization of hardware ext. interrupts
 * \param *handler pointer to a function which handle occured interrupt.
 * \return nothing
 */
void extInterruptINIT(void (*handler)(void))
{
	/* Set function pointer */
	 int_handler = handler;
	/* Initialize external interrupt on pin INT0 on failing edge */
	 EICRA |= (1 << ISC01);
	 EIMSK |= (1 << INT0);
}
/* System interrupt handler */
ISR(INT0_vect)
{
	int_handler();
}
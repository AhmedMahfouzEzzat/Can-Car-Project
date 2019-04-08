/*
 * Ultrasonic.c
 *
 * Created: 08/04/2019 12:27:50 PM
 *  Author: Ahmed
 */ 
#define F_CPU 16000000UL
#include "Ultrasonic.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void INIT_Ultrasonic()
{
	DDRB|=(1<<trig);
	cli();
	TIMSK1 = (1 << TOIE1);	/* Enable Timer1 overflow interrupts */
	TCCR1A = 0;		/* Set all bit to zero Normal operation */
	sei();			/* Enable global interrupt */
	
}
double Get_distance()
{
	long count;
	double distance;
	PORTD &= (~(1 << PD3));
	_delay_us(2);
	PORTD |= (1 << PD3);
	_delay_us(10);
	PORTD &= (~(1 << PD3));
	TCNT1 = 0;   	// Clear Timer counter 
	TCCR1B = 0x41;	// Capture on rising edge, No prescaler
	TIFR1 = 1<<ICF1;	// Clear ICP flag (Input Capture flag) 
	TIFR1 = 1<<TOV1;	// Clear Timer Overflow flag 

	/*Calculate width of Echo by Input Capture (ICP) */
		
	while ((TIFR1 & (1 << ICF1)) == 0);/* Wait for rising edge */
	TCNT1 = 0;	    /* Clear Timer counter */
	TCCR1B = 0x01;	/* Capture on falling edge, No prescaler */
	TIFR1 = 1<<ICF1;	/* Clear ICP flag (Input Capture flag) */
	TIFR1 = 1<<TOV1;	/* Clear Timer Overflow flag */
	TimerOverflow = 0;/* Clear Timer overflow count */

	while ((TIFR1 & (1 << ICF1)) == 0);/* Wait for falling edge */
	count = ICR1 + (65535 * TimerOverflow);	/* Take count */
	/* 16MHz Timer freq, sound speed =343 m/s */
	distance = (double)count / 932.94;
	return distance;	
}



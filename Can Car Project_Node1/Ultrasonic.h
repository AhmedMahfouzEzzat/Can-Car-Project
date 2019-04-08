/*
 * Ultrasonic.h
 *
 * Created: 08/04/2019 12:28:25 PM
 *  Author: Ahmed
 */ 


#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#define echo 0
#define trig 1

int TimerOverflow = 0;

ISR(TIMER1_OVF_vect)
{
	TimerOverflow++;	/* Increment Timer Overflow count */
}

#endif /* ULTRASONIC_H_ */
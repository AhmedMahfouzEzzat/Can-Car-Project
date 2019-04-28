#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "AVR_CAN.h"
#include "ADC.h"
#include "hc-sr04.h"
int flag =0;
int main(void)
{
	INIT_ADC();
	init_ultrasonic_sensor();
	initMCP2515();
	DDRB=0xff;
	uint16_t ADC_Value;
	int temperature = 0;
	unsigned char buffer[10];
	float dest=0;
	unsigned char buffer_size=0;
	//Button SetUp Using Interrupt
	DDRD &= ~ (1<<2); 
	EIMSK =(1<<INT1);
	EICRA = 0x02;
	sei();

	while(1)
	{
		if(flag)
		{		
			ADC_Value = Analog_Read(0); 
			temperature =(ADC_Value*500)/1023; 
			dest =read_dist();
		
			if(temperature>=30)
			{
				itoa(temperature,(char*)buffer,10);
				buffer_size =sizeof(buffer)/sizeof(char);
		
			   sendCANmsg(0,0x06,buffer,buffer_size);
			}
			if(dest < 20 )
			{
				itoa(dest,(char*)buffer,10);
				buffer_size =sizeof(buffer)/sizeof(char);
		
			   sendCANmsg(0,0x05,buffer,buffer_size);
			}
		}		
	}
}

ISR(INT1_vect)
{
	flag!=flag; 
	sendCANmsg(0,0x07,flag,1);
}	


/*
 * Can_Car_Project_Node2.c
 *
 * Created: 02/04/2019 05:03:46 AM
 *  Author: Ahmed and Hussam
 */ 


/*
PIN MAP:
B2 -> B5 SPI
C0 -> C5 LCD

D0,D1 PWM Dir 1
D4,D7 PWM Dir 2
D2,D3 INT
D5 OC0B PWM
D6 OC0A PWM

*/
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include "AVR_CAN.h"
#include "LCD.h"
#include "PWM.h"


int main(void)
{
	unsigned char isON = 1;
	unsigned char dutyCycle0 = 255;
	unsigned char dutyCycle1 = 255;
	unsigned char i = 0;
	long id = 0 ;
	initMCP2515();
	LCD_Init();
	PWM_start();
	sei();
	LCD_String("MOTOR1");
	LCD_String("X");
	LCD_String_xy(1,0,"MOTOR2");
	LCD_String("Y");
	while(1) 
	{
		if(msgReceived) 
		{
			LCD_Clear();
			id = ((unsigned char)(getId<<8));
			id += (unsigned char)getId;
			if(id == 0x05) //Ultra sonic id
			{
				LCD_Clear();
				LCD_String("USonicVal:");
				
				LCD_String_xy(1,0," ");
				for(i=0; i<getLength; i++)
				{	
					LCD_Char(getData(i));
				}	
				PWM0_setDutyCycle(0);
				PWM1_setDutyCycle(0);
			}
			else if(id == 0x06) //Temperature id
			{
				LCD_Clear();
				LCD_String("TempVal:");
				
				LCD_String_xy(1,0," ");
				for(i=0; i<getLength; i++)
				{	
					LCD_Char(getData(i));
				}	
				PWM0_setDutyCycle(0);
				PWM1_setDutyCycle(0);
			}
			else if(id == 0x07) //Button id
			{
				if (isON)
				{
					PWM0_setDutyCycle(0);
					PWM1_setDutyCycle(0);
					LCD_Clear();
					isON = 0;
				}
				else
				{
					LCD_String("MOTOR1");
					LCD_String("X");
					LCD_String_xy(1,0,"MOTOR2");
					LCD_String("Y");
					PWM0_setDutyCycle(dutyCycle0);
					PWM1_setDutyCycle(dutyCycle1);
					isON = 1;
				}
				break;
			}			
		}
	}



	return 0;
}
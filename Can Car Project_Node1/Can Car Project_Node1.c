#include <avr/io.h>
#include <util/delay.h>
#include <AVR_CAN.h>
#include "ADC.h"
#include "LCD.h"

int main(void)
{
	
DDRB=0xff;
uint16_t ADC_Value;
int ADC_DigitalValue;
char buffer[10];

//Button SetUp Using Interrupt
DDRD |= 0x02; 
EIMSK =(1<<INT0);
EICRA = 0x02;
sei();

INIT_ADC();
LCD_Init ();
LCD_Clear();
LCD_String_xy (0,0,"");
_delay_ms(100);

while(1)
{

ADC_Value = Analog_Read(0); 
ADC_DigitalValue =(ADC_Value*500)/1023; 


if(ADC_DigitalValue>=30)
   {
	LCD_Clear();
    LCD_String_xy (0,1,"");
	itoa(ADC_DigitalValue,buffer,10);
    LCD_String("Buzzer ON");
	LCD_String(buffer);


    }

else

   {

	LCD_Clear();
    LCD_String_xy (0,1,"");
    itoa(ADC_DigitalValue,buffer,10);
    LCD_String("Buzzer OFF");
	LCD_String(buffer);

    }
  }
}

ISR(INT0_vect){
	
	// SEND CAN MSG
}

	


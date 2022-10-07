/*
 * 

 * Author : JoeyMancuso
 */ 
//#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>
#include "USART.h"
void initalize();
uint8_t getDigit(int number, int index);
void showNumber(int index , int digit);
uint8_t numbers[] = {0b00000011, 0b10011111,0b00100101, 0b00001101, 0b10011001, 0b01001001, 0b01000001,0b00011111, 0b00000001, 0b00001001};
uint8_t debounce(uint8_t port, uint8_t pin);
void buttonPress(int dispVal);
#define setBit(x,y) (x |= (1<<(y)))
#define TOGGLE(x,y) (x ^= (1<<(y)))
#define UNSET(x,y) (x &= ~(1<<(y)))
#define bitcheck(byte,nbit) ((byte) &   (1<<(nbit)))




int main(void)
{
	//could makea function to init
	char strV[5];
	initalize();
	//initUSART();
	DDRC = 0b00000000;
	setBit(PORTC,5); // pull up resistor
	DDRB = 0xFF;
	DDRD = 0xFF;
	int dispVal;
    /* Replace with your application code */
    while (1) 
    {
	
		ADCSRA |= (1<<ADSC); //turns on ADC
		loop_until_bit_is_set(ADCSRA, ADIF);// adif having a value of 1 means conversion is done, so loop until adif in ADSXRA is set to 1 
		
		if (ADC >= 0 && ADC <= 100)
		{
			dispVal = 100;
		}
		if (ADC >= 101&& ADC <= 200)
		{
			dispVal = 500;
		}
		if (ADC >= 201 && ADC <= 300)
		{
			dispVal = 1000;
		}
		if (ADC >= 301 && ADC <= 400)
		{
			dispVal = 1500;
		}
		if (ADC >= 401 && ADC <= 500)
		{ dispVal = 2000;}
		if (ADC >= 501 && ADC <= 600)
		{
			dispVal = 2500;
		}
		if (ADC >= 601 && ADC <= 700)
		{
			dispVal = 3000;
		}
		if (ADC >= 701 && ADC <= 800)
		{
			dispVal = 3500;
		}
		if (ADC >= 801 && ADC <= 900)
		{
			dispVal = 4000;
		}
	if (ADC >= 901 && ADC <= 1000)
		{
			dispVal = 4500;
		}
		if (ADC >= 1001 && ADC <= 1024)
		{
			dispVal = 5000;
		}
		
		buttonPress(dispVal);
		
		// main number display loop
		uint8_t num = getDigit(dispVal,1000);
		 showNumber(num,4);
		 _delay_ms(5);
		 num = getDigit(dispVal,100);
		 showNumber(num,3);
		 _delay_ms(5);
		 num = getDigit(dispVal, 10);
		 showNumber(num, 2);
		 _delay_ms(5);
		 num = getDigit(dispVal,1);
		showNumber(num,1);
		_delay_ms(5) ;
		
		
		
    }
}

void initalize(){
	ADMUX  |= (1<< REFS0); // think this sets port 0 to ADC, unsure, ask for help and refer // 01 sets vcc voltage power
	ADCSRA |= (1<<ADEN); // not clear what this is doing, make a note when you learn 
	ADCSRA |= (1<< ADPS2) | (1<<ADPS1) | (1<<ADPS0); // these all get set to one for accuracy
//	ADCSRA |= (1<< ADPS1);
	//ADCSRA |= (1<<ADPS0);
	//optional based on port needed/selcted
	//refer to page 317
	//if not using port 0, refer to this code and the polling video or ask kk to be sure 
	//admux |= (1<<MUX2) | (1<<mux1) | (1<<MUX0);
	
}


uint8_t getDigit(int number, int index){
	if(index == 1){
		return number %10;
	}
	else if (index == 10)
	{
		return (number%100) /10;
	}
	else if(index == 100)
	{
		return (number%1000)/100;
	}
	else if (index == 1000)
	{
		return number/1000;
	}
	return 0;
}
void showNumber(int redNum , int digit){
	PORTB &= 0b00110000; //adjustable // possible break point?
	PORTD = numbers[redNum];
	setBit(PORTB,digit-1); // ORing portb @ bit of digit provided 
	
}

uint8_t debounce(uint8_t port, uint8_t pin){
	if (bit_is_clear(port, pin) )
	{
		_delay_ms(5);
		if(bit_is_clear(port,pin))
		{
			return 1; // button is clear
		}
	}
	return 0; //button is set
}

void buttonPress(int dispVal){
if (debounce(PINC,5))
{
	UNSET(ADCSRA,ADSC);
	int d = 1000;
	int num;
	while (dispVal != 0) // countdown
	{  //for fun, how would i make this into a loop? num = getdigit(dispval, d)  d would have to be dynamically dvided depending on the iteration
		dispVal--;
		
		num = getDigit(dispVal,1000);
		showNumber(num,4);
		_delay_ms(5);
		num = getDigit(dispVal,100);
		showNumber(num,3);
		_delay_ms(5);
		num = getDigit(dispVal, 10);
		showNumber(num, 2);
		_delay_ms(5);
		num = getDigit(dispVal,1);
		showNumber(num,1);
		_delay_ms(5) ;
		
		if (dispVal == 0)
		{
			for (int i = 0; i<10; i++) // works, blinker ight
			{
				
				
				showNumber(0,1);
				_delay_ms(100);
				showNumber(0,2);
				_delay_ms(100);
				showNumber(0,3);
				_delay_ms(100);
				showNumber(0,4);
				_delay_ms(100);
				setBit(PORTB, 4);
				
				_delay_ms(333);
				//UNSET(PORTB,4);
				PORTB = 0;
				_delay_ms(333);
				
				
				
			}
			
			
			setBit(ADCSRA,ADSC);
			break;
		}
		
	}
	}
}

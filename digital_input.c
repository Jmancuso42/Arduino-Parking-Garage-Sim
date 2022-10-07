/*
* GccApplication1.c
*

* Author : JoeyMancuso
*/
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
//#include "USART.h"
#include <stdbool.h>
#define green 5
#define red 4
//#define BV(bit)(1<<(bit));
//#define setbit2(byte,bit) (byte&= ~BV(bit));


#define setBit(x,y) (x |= (1<<(y)))
#define TOGGLE(x,y) (x ^= (1<<(y)))
#define UNSET(x,y) (x &= ~(1<<(y)))
#define bitcheck(byte,nbit) ((byte) &   (1<<(nbit)))


uint8_t debounce(uint8_t port, uint8_t pin);
void toggle(uint8_t num,int pos);
void unset(uint8_t num,int pos);
void setbit(uint8_t byte, uint8_t bit);
void showNumber(uint8_t index, uint8_t digit);
uint8_t getDigit(uint8_t number, uint8_t index);

uint8_t numbers[] = {0b00000011, 0b10011111,0b00100101, 0b00001101, 0b10011001, 0b01001001, 0b01000001,0b00011111, 0b00000001, 0b00001001};

int main(void)
{
	
	//initUSART();
	DDRD = 0xFF;
	DDRB = 0xFF; //0b11111111;
	DDRC = 0b00000000; // 1s are output, 0s are input, i think  // i believe this is setting the selected slots to be input devices, but i could be wrong and this may be a source of error
	PORTC = 0b00001111; //important! pullup resistor!
	
	
	uint8_t minute = 0;
	uint8_t hours = 0;
	
	/*uint8_t lastNum = minute%10; //last number
	uint8_t thirdNum = minute/10;
	uint8_t firstNum = hours/10;
	uint8_t secondNum = hours%10;
	*/
	
	int num;
	bool amPm = false; //false
	bool isPm = false;
	
	//green 0b00100000; //5th bit, 6th digit a
	//red	0b00010000; 4th bit , 5th digit
	//      0b11101000 outer right in binary
	//		0b11011000 what I think red light + outer right might be.
	
	
	//PORTB =  0b00010000; 	//	red light
	//PORTB = 0b11011000; //redlight outerlight, doesn't work, green light still going on
	//PORTB = 0b11111000; // forgot what this is for
	//PORTB = outerRight;
	
	
	// note to self 001 port b turns on green light
	
	
	while (1)
	{		//ampm switch setting
		if (bit_is_clear(PINC, 0)){
			
			setBit(PORTB,4); //red light
			UNSET(PORTB,5); //green off
			_delay_ms(5);
			if (amPm == false)
			{
				
				//hours -= 12;
				amPm = true;
				
				if (hours == 0)
				{
					hours = 12;
					isPm = false;    //kk added false
				}
				else if (hours == 12)   //kk changed to else if
				{
					isPm = true;
				}
				
				
				else if (hours>12)    //kk deleted a ; at the end of this statement and changed to else if
				{
					isPm = true;
					hours -= 12;
					//turn on dot
				}
				else
				{
					isPm = false;    //kk added else
				}
				
				//hours +=12
			}//KKmoved this ending curly brace
		}
		
		
		else // if (bit_is_clear(PINC,1))
		{
			//24 hr
			
			setBit(PORTB,5); // green light
			UNSET(PORTB,4); //turn off red light
			//PORTB = 0b00100000;  //the green
			if (amPm == true)
			{
				amPm = false;
				
				if (hours == 24)
				{
					hours = 0;
				}
				
				
				if (hours > 12 )//|| ispm == true;
				{
					hours += 12;
				}
			}
			
		}
		
		if (debounce(PINC,2)) // left button (hours)
		{
			hours ++;
			
			//	printByte(hours);
			if (amPm && hours>12){
				isPm = true;
				// bitshift a dot on.
				
				if (hours == 24)
				{
					hours = 0;
					if (amPm)
					{
						isPm = false;
					}
				}
				
				
				
			}
			_delay_ms(100);
		}
		
		//minute button
		if(debounce(PINC, 3)){
			/*	printByte(minute);
			printString("\n portb: ");
			printBinaryByte(PORTB);
			*/
			
			
			
			if(minute >59){
				minute = 0;
				//hours++;
				//printString("loop around if");
				
			}
			else{
				minute++;
				//	printByte(minute);
				//printString("\n else minute increment");
				//_delay_ms(100);
			}
			_delay_ms(100);
		}
		
		num= getDigit(hours,1);
		showNumber(num,3);
		_delay_ms(5);
		num = getDigit(hours,10);
		showNumber(num,4);
		_delay_ms(5);
		num = getDigit(minute,1);
		showNumber(num, 1);

/*		if(amPm == true && isPm == true){
			UNSET(PORTD,0);
		}
		else {
			setBit(PORTD,0);
		}*/
		_delay_ms(5);
		num = getDigit(minute,10);
		showNumber(num,2);
		_delay_ms(5);
		
		
		
	}
}

void toggle(uint8_t num,int pos)
{
	num ^= (1 << pos);
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

void unset(uint8_t num,int pos)
{
	//Second step is to bitwise and this  number with given number
	num &= (~(1 << pos));
}

void setbit(uint8_t byte, uint8_t bit){
	(byte |= (bit));
}


void showNumber(uint8_t index , uint8_t digit){
	PORTB &= 0b00110000;
	PORTD = numbers[index];
	setBit(PORTB,digit-1);
	
}

uint8_t getDigit(uint8_t number, uint8_t index){
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


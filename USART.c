/*
 
 * Author : JoeyMancuso
 */ 

#include <avr/io.h>
#include "USART.h"
#include <stdlib.h>
#include <util/delay.h>


int main(void)
{
	char sVal[6];
		
	initUSART();
	DDRB  = 0b00111111;
	PORTB = 0b00010101;
	
    while (1) 
    {
		
		
		
		printString("Enter a number \n");
		uint8_t val = getNumber();
		itoa(val,sVal,10);
		
		//printString("Enter a thing \n");
		
		printString("Value entered: \t");
		printByte(val);
		printString("\n");
		
		
		printString("\n evaluating val \n");
		switch(val){
			case 1:
			PORTB ^= (1<<1) | (1<<0); // right red
			break;
			case 2:
			PORTB ^= (1<<1) |(1<<2) | (1<<0);
			break;
			case 3:
			 PORTB ^= (1<<1) |(1<<2) | (1<<3);
			break;
			case 4:
			 PORTB ^= (1<<2) | (1<<3)| (1<<4);
			break;
			case 5:
			PORTB ^= (1<< 3) | (1<<4) |(1<<5);
			break;
			
			case 6:
			PORTB ^= (1<<4) | (1<<5);
			break;
			default:
			printString("\n Not a valid number");
			break;
		}
		
		if (PORTB == 0b00000000){
			printString("You win, Genius");
		
			PORTB = 0b00101010;
			_delay_ms(500);
			PORTB = 0b00010101;
			_delay_ms(500);
			PORTB = 0b00101010;
			_delay_ms(500);
			PORTB = 0b00010101;
			_delay_ms(500);
			PORTB = 0b00101010;	
		}
		
		printString("\n Port b binary \t");
		printBinaryByte(PORTB);
		
		printString("End \n");
		}
	return (0);
}


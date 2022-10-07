/*
 * KeyPad.c
 *
 
 * Author : JoeyMancuso
 */ 

#include <avr/io.h>
#include "USART.h"
#include <avr/sfr_defs.h>
#include <stdio.h>
#include <avr/delay.h>
#define F_CPU 16000000;
char keyPadArray[4][4];
uint8_t rowScan();
uint8_t colScan();
#include <stdbool.h>
void buttonGet();
void setRowInput();
void setColInput();
bool buttonPushed = false;
int main(void)
{	
	
	
	
	initUSART();

    while (1) 
    {
		buttonGet();
    }
}



void buttonGet(){
	uint8_t colVal = colScan();
	uint8_t rowVal = rowScan();
	char buttonHolder [20];
	char debugHolder [3];
	//[][] left to right is row, then column
	
	/*
	{{'1','2','3','A'},
	{'4','5','6','B'},
	{'7','8','9','C'},
	{'*','0','#','D'}};
	*/
	
	char keyPadArray[4][4] = {{'D','#','0','*'},
							  {'C','9','8','7'},
							  {'B','6','5','4'},
							  {'A','3','2','1'}};
	
	
	//Tried debugging by just printing out the row val and col val before they're
	// assigned to keydigit, my inital test showed that col is defaulting to 0 most of the time
	// row is defaulting to 8 which means the scan loop failed to find a clear bit
	
	//UPDATE: I messed with the init, now the Rows will at least show me almost the right row ( it notes the top row as 3, but I want it to be 0, so it's flipped)
	
	
	//UPDATE 2 I somehow got the cols to read a value that isn't 8, dont remember how
	//It seems to work now except the values for the buttons aren't correct
	// my guess in the morning has to do with the for loops: one of them starts at 4, so I need to account for the moving parts in the loop as problem source
	//they're not necessarily starting at the same thing.
	sprintf(debugHolder,"  R: %u  ",rowVal);
	printString(debugHolder);
	_delay_ms(150);
	sprintf(debugHolder, "  C: %u", colVal);
	printString(debugHolder);
	_delay_ms(150);
	
	
	
	
	
	char keyDigit = keyPadArray[rowVal][colVal];
	
	//if(rowVal != 9 && colVal != 9) 
	
	if (rowVal !=69 && colVal != 69 ) //rowVal != 8 && colVal != 8)
	{
	
	sprintf(buttonHolder," key: %c \n",keyDigit);
	printString(buttonHolder);
	_delay_ms(50);
	}
	
}


uint8_t rowScan(){ //point of error if i get the array wrong 
	setRowInput();
	_delay_ms(10);
	char holder[80];
	for (uint8_t i = 0; i < 4; i++)
	{
		if(bit_is_clear(PINB,i)){
//			printString(" ric  "); 
		 	//sprintf(holder,"  rI: %u  ",i); //supposed to show what iteration of i detected that the pin that is clear in row scan
		//	printString(holder);
		buttonPushed = true;
			return i;
			
		}
		
	}
	buttonPushed = false;
	return 69;
	
	
}

uint8_t colScan(){
		setColInput();
		_delay_ms(10);
		char holder[80];
		for (uint8_t i = 4; i<8; i++)
		{
			if (bit_is_clear(PIND,i))
			{
				
				//printString(" c ");
				//sprintf(holder,"  cI:  %u  ",i ); //tests to see what iteration i is during the column scan
				//printString(holder);
				buttonPushed = true;	
				return i-4;
			}
			
		
		
		}
		buttonPushed = false;
		return 69;
		

	
}
//upper four of D are only ones that need to be touched for pind,(leftmost 4)
//lower four of B are the only active (rightmost 4)
// b is rows d is columns 
//left is rows right is columns   is left B COLU,MS D is right // top B is columns // bottom is D Row 
void setColInput(){
	DDRD = 0b00001111; //input pins, 0 =input, rows is d and sets 4 bits to output
	DDRB = 0b00001111; //output pins  cols// column is b and set 4 bits to input 
	PORTB = 0; 
	PORTD = 0b11110000;// pullup resistors on input
			
	}
void setRowInput(){
	//sets rows as input 
	DDRD = 0b11110000; //output pins  row    d is  // sets rows 
	DDRB = 0b11110000; //input pins cols     b is cols // sets cols to output 
	PORTD = 0;
	PORTB = 0b00001111; //pull up resistors on input
}

/*
 * LCD.c
 *

 * Author : JoeyMancuso
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <stdlib.h>
#include <avr/delay.h>
#include <avr/sfr_defs.h>
#include <stdio.h>


#define SetBit(x,y)         (x|=(1<<y))
#define ClrBit(x,y)         (x&=~(1<<y))
#define ToggleBit(x,y)      (x^=(1<<y))
#define FlipBit(x,y)      (x^=(1<<y)) // Same as ToggleBit.
#define TestBit(x,y)      (x&(1<<y))

#define RS PD1	
#define E  PD3

void lcdCommand(uint8_t command);
void lcdData(uint8_t data);
void lcdInit();
void lcdGoToLine1();
void lcdGoToLine2();
void lcdPrint(char*str);
 main(void)
{
	char str[80];
	char iChar[20];
	 
	
	lcdInit();
	
	int i = 0; 
    /* Replace with your application code */
    while (1) 
    {
		
		

		lcdGoToLine1();
		itoa(i,iChar,10);
	    
		
		lcdPrint("Bottles of beer");
		lcdGoToLine2();
		sprintf(str,"On the wall: %s "	, iChar );
		lcdPrint(str);
		i--;
		if (i <= 0)
		{
			i = 99;
		}
		
    }
}

void lcdCommand(uint8_t command){
	PORTD = command & 0xF0; // masks lower nibble
	ClrBit(PORTD,RS);
	SetBit(PORTD,E);
	_delay_us(1);
	ClrBit(PORTD, E);
	_delay_us(100);
	
	PORTD = command << 4; // shift to lower nibble 
	//lower nibble 
	ClrBit(PORTD,RS);
	SetBit(PORTD,E);
	_delay_us(1);
	ClrBit(PORTD,E);
	_delay_us(100);

	
}

void lcdData(uint8_t data){
	PORTD = data & 0xF0;
	
	SetBit(PORTD,RS);
	SetBit(PORTD,E);
	_delay_us(1);
	ClrBit(PORTD,E);
	_delay_us(100);
	
	
	PORTD = data << 4; // shift to lower nibble
	//lower nibble
	SetBit(PORTD,RS);
	SetBit(PORTD,E);
	_delay_us(1);
	ClrBit(PORTD,E);
	_delay_us(100);

	
}

void lcdInit(){
	DDRD = 0b11111010;
	ClrBit(PORTD,E);
	lcdCommand(0x33);
	lcdCommand(0x32);
	lcdCommand(0x28);
	lcdCommand(0x0E);
	lcdCommand(0x01);
	lcdCommand(0x06);
	
	
}

void lcdGoToLine1(){
	lcdCommand(0x80);
	_delay_ms(100);
	
}
void lcdGoToLine2(){
	lcdCommand(0xC0);
	_delay_ms(100);
}
void lcdPrint(char*str){
	uint8_t i = 0;
	
	while (str[i] != 0){
		lcdData(str[i]);
		i++;
		}
}

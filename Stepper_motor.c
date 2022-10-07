/*
 * stepperMotor.c
 *
 *
 * Author : JoeyMancuso
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "USART.h"

 uint8_t array[] = {0b1000, 0b1100, 0b0100, 0b0110,0b0010, 0b0011,0b0001,0b1001};
static void eightBitClockwardSpin(void);
static void eightBitCounterSpin(void);
static void fourBitClockwardSpin(void);
static void fourBitCounterSpin(void);
static void thirdStepperCCW(void);
static void thirdStepperCW(void);
static void eightbitDumbSpin(void);


static void eightbitDumbSpin(void){
		PORTB = array[0];
		_delay_ms(5);
		PORTB = array[1];
		_delay_ms(5);
		PORTB = array[2];
		_delay_ms(5);
		PORTB = array[3];
		_delay_ms(5);
		PORTB = array[4];
		_delay_ms(5);
		PORTB = array[5];
		_delay_ms(5);
		PORTB = array[6];
		_delay_ms(5);
		PORTB = array[7];
		
	};

static void eightBitClockwardSpin(void){

	for (int j = 0; j<300;j++)
	{
	for (int i = 0; i< 7;i++)
	{
		printBinaryByte(PORTB );
		printString(" ");
		PORTB = array[i];
		_delay_ms(1);
	}
	}
}

static void eightBitCounterSpin(void){
	
	for (int j = 0; j<300;j++)
	{
	
	for (int i = 7; i>-1;i--)
	{
		PORTB = array[i];
		_delay_ms(5);
	}
	}
}

static void thirdStepperCW(void){
	for (int j = 0; j<300;j++)
	{
	for (int i = 0; i <7;i += 3)
	{
		PORTB = array[i];
		_delay_ms(5);
	}
	}
}


static void thirdStepperCCW(void){
	
	for (int j = 0; j<300;j++)
	{
	for (int i = 7; i> -1;i -= 3)
	{	
		
		PORTB = array[i];
		_delay_ms(5);
	}
	}
}


static void fourBitClockwardSpin(void){
	
	for (int j = 0; j<300;j++)
	{
	for (int i = 0; i< 7;i+=2)
	{
		PORTB = array[i];
		_delay_ms(5);
	}
	}
}

static void fourBitCounterSpin(void){
	
	
	for (int j = 0; j<300;j++)
	{
	for (int i = 7; i> -1;i-=2)
	{
		PORTB = array[i];
			_delay_ms(5);
	}
	}
}


int main(void)
{
	
	DDRB = 0xFF;
	initUSART();
   while (1) 
    {
		printString("top ");
		eightBitClockwardSpin();
		_delay_ms(1000);
		eightBitCounterSpin();
		_delay_ms(1000);
		fourBitClockwardSpin();
		_delay_ms(1000);
		fourBitCounterSpin();
		_delay_ms(1000);
		thirdStepperCW();
		_delay_ms(1000);
		thirdStepperCCW();
		_delay_ms(1000);	
	
	}
}


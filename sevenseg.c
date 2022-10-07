/*
 * GccApplication1.c
 *

 * Author : JoeyMancuso
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
// portb bit 0 is the far right num, bit 1 is inner right, 2 is inner left 3 is outer left, 4 might be a light 


uint8_t extractTool (uint8_t n){
	uint8_t i,len;
	 char temp[5];
	 //assig number aas string in char pointer
	 sprintf((char*)temp,"%u",n);
	 
	 //extract and print each digits
	 len= strlen(temp);
	 for(i=0; i<3; i++)
	 temp[0];}
int main(void)
{
	
	DDRB = 0XFF;
	DDRD = 0XFF;
	#
	
	
	uint8_t numbers[] = {0b00000011, 0b10011111,0b00100101, 0b00001101, 0b10011001, 0b01001001, 0b01000001,0b00011111, 0b00000001, 0b00001001};
	//PORTB = 0b11110001;
	PORTB = 0b11100011; // last two numbers on E3 
	//portb = 0b11100001 last number on  E1
	//portb = 0b11100010 = e2  is only inner right
	//portb = 0b11100100 inner left e4
	//port b =0b11101000 outer left e8
	//port b =0b11110000 = red light on nothing else = f0
	PORTD = numbers[2];
	#define outerLeft  0xE8;
	#define innerLeft 0xE4;
	#define innerRight  0xE2;
	#define outerRight  0xE1;
	#define one numbers[1];
	
	
	int n = 1500;
	//uint8_t displaySeg[] = {outerLeft,innerLeft, innerRight, outerRight};
	
	uint8_t lightSeg[] = {0xE8, 0xE4, 0xE2, 0xE1};
	while (1) 
    { 			//how would I get the port to shift by one every iteration?
			//portD is the numbers
				while (n != 0) // outer loop. would be great to be able to extrapolate each digit from n count into variables and just  
				{	
					n--;
					
					}
					
					uint8_t l = n%10; //last number
					uint8_t t = (n %100)/10;	//3rd 
					uint8_t s = (n/100)%10; //2nd 
					_delay_ms(5);
					if (n <= 0)
					{
						for (int i = 0; i<6; i ++) //bomb explosion
						{	
							PORTB = 0;
							PORTD = 0;
							_delay_ms(500);
							PORTD = 0xFF;
							PORTB = 0xFF;
							_delay_ms(1000);
							
						}
						_delay_ms(500);
						n = 1500;
					}
						if (n < 1000) // toggle thousands light (left most)
						{	
							_delay_ms(5);
							PORTB = outerLeft;
							PORTD = numbers[0];
						} 
						else{	PORTD = numbers[1]; // else leave it as one 
								PORTB = outerLeft;
								_delay_ms(5);
					}
							
						_delay_ms(5); // 2nd number hundreds lights 
							PORTB = innerLeft;
							PORTD = numbers[s];
							
							 _delay_ms(5);// 3rd digits place
							 PORTB = innerRight;
							 PORTD = numbers[t];
							
							 _delay_ms(5);
							 PORTB = outerRight;
							 PORTD = numbers[l];	
		}
						
	}
}
														
/*
					
	}
	}
				
			
			//PORTB = 0xFf;
			//PORTD = 0b11111000; //red light
			
		//PORTB = 0b00001111;	
		*/
	
			




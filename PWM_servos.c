/*
 * servoDemo.c
 *
 
 * Author : JoeyMancuso
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>


#define pulse_min 800
#define PULSE_MAX 4000
#define PULSE_MID 1900
// is the number the result of the formula? what kind of math do we need to do
// how to determine pulse lengths? 
// gets the servo to pulse a bit

static void initTimer1servo(void);
static void showOff(void);

static void showOff(void){
	OCR1A = PULSE_MID;
	_delay_ms(1500);
	
	OCR1A = pulse_min; //oc1ra sets the duty cycle 
	_delay_ms(1500);
	
	OCR1A = pulse_min*3; //45 degress
	_delay_ms(1500);
	
	OCR1A = PULSE_MID;
	_delay_ms(1500);
	
	OCR1A = PULSE_MAX/4;
	_delay_ms(1500);
	
	OCR1A = PULSE_MAX;
	_delay_ms(1500);
	
	
}

static void initTimer1servo(void){
	TCCR1A |= (1<< WGM11); // this and next line set up the fast pwm waveform generator 
	TCCR1B |= (1<< WGM12) | (1<<WGM13);
	TCCR1B |= (1<<CS11); // 8 bit prescaling 
	ICR1 = 40000; // top value = 20ms.   fcpu/presclaer/50
	TCCR1A |= (1<<COM1A1);// direct output on pb1/ oc1a 
	DDRB |= (1<<1); //set pb1 to be output 
	
}

int main(void)
{	
	initTimer1servo();
	showOff();
    
    while (1) 
    {
		
    }
}


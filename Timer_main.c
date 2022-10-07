/*
 * 
 * Author : JoeyMancuso
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
//#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>
//#include "USART.h"

#define setBit(x,y) (x |= (1<<(y)))
#define TOGGLE(x,y) (x ^= (1<<(y)))
#define UNSET(x,y) (x &= ~(1<<(y)))

static void initTimer(void);
static void playNote(uint8_t waveLength, uint8_t duration);
static void initScaler64(void);
static void initScaler8(void);
static void initScaler256(void);
static void initScaler1024(void);
static void ascendingTone(void);
static void descendingTone(void);
static void playFourForMeSammy(void);

int main(void)
{
	initTimer();
	initScaler64();
	TCCR2B |= (1<< CS21) | (1<<CS20);
	

    while (1) 
    {

		//initTimer();
	  playFourForMeSammy();
		_delay_ms(2000);
		ascendingTone();
		_delay_ms(2000);
		descendingTone();
		_delay_ms(2000);
		
		
    }
}
// not concrete on all these registers or what they're doing exactly

static void initTimer(void){
	TCCR2A |= (1<< WGM01); // timer 0 CTC mode 
	TCCR2A |= (1<< COM1A0); // toggles pin each cycle?  // what is this doing //
	TCCR2B = 0;
//	TCCR2B |= (1<< CS21) | (1<<CS20);  // 64 bit prescaler
}

static void initScaler64(void){
	TCCR2B = 0;
	TCCR2B |= (1<< CS21) | (1<<CS20);
}
static void initScaler256(void){
	//TCCR0A |= (1<< WGM01);
	//TCCR0A |= (1<< COM0A0); //toggles oc0a on compare match 
	TCCR2B = 0;
	TCCR2B |= (1<<CS22) |(0 << CS21) | (0 << CS20);
	
}
static void initScaler1024(void){
	//TCCR0A |= (1<< WGM01); // these two lines might be unnecessary
	//TCCR0A |= (1<< COM0A0);
	TCCR2B = 0;
	TCCR2B |= (1<< CS22 )| (1 <<CS21) | (1<< CS20);// idk if these registers are valid registers for prescaling tbh 
	
}
static void initScaler8(void){
	TCCR2B = 0;
	TCCR2B |= (1<<CS21) | (0 << CS20);
}
static void playNote(uint8_t waveLength, uint8_t duration){
	OCR2A = waveLength;
	setBit(DDRB,3); // change this if needed
	
	while(duration){
		_delay_ms(1);
		duration--;
	}
	UNSET(DDRB,3); //
	
}

static void ascendingTone(void){
	initScaler8();
	
	playNote(250,250);
	playNote(200,250);
	//initScaler256();
	playNote(150, 200);
	playNote(200,200);
	
}

static void descendingTone(void){
	initScaler8();
	playNote(200,250);
	playNote(250,200);
	
}

static void playFourForMeSammy(){
	initScaler256();
	playNote(250,200);
	_delay_ms(500);
	
	initScaler8();
	playNote(250,200);
	_delay_ms(500);
	
	initScaler1024();
	playNote(250,200);
	_delay_ms(500);
	
	initScaler64();
	playNote(250,200);
	_delay_ms(500);
}

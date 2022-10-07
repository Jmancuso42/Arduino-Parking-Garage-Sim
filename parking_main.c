/*
 * arduinoProject.c
 * Uses my setup to act as a parking garage
 * Author : JoeyMancuso
 */ 
//fCPU definition wasn't working 
#include <avr/io.h>
#include <stdbool.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>



/*
***Notes***

//oc1ra sets the duty cycle {
	for example: OC1RA = PUlSE_MaX sets the duty cycle to run for the length of whatever value PUlseMax is, for me 4000, turning the servo that amount 
}





to handle the servo, we can do 3 seperate functions OR 1 function thats conditional{
	
	if counter == 15 { set variable a 0 }
		else
	
	if counter isn't at max{
		a variable is set to 1, pass into servo function 
		
		servo on receivning a 1 does a full length open
		if it received a zero, it shouldn't open
		



*/

/*
	SET PULLUP RESISTORS
	SET DDRXs 
	SET 
*/

/* 
	questions
What does bit check do?

what am i doing with DDRX exactly, what's goin on



*/




#define PULSE_MIN 800
#define PULSE_MAX 4800
#define PULSE_MID 1900
#define SETBIT(x,y) (x |= (1<<(y)))
#define TOGGLE(x,y) (x ^= (1<<(y)))
#define UNSET(x,y) (x &= ~(1<<(y)))
#define bitcheck(byte,nbit) ((byte) &   (1<<(nbit)))

#define initCarCapacity 15
#define maxCarLim 0
#define redOn 
#define redLight 4
#define greenLight 5
#define open 1
#define closed 0
#define isUp 3
#define inButtonPin 2
#define outButtonPin 0 


static void quarterTurnCw();

static void initTimer();
static void initScaler8();
static void initScaler64();
static void initScaler256();
static void initScaler1024();
static void initTimer1servo();

uint8_t debounce(uint8_t port, uint8_t pin);
void inButtonPush();
void outButtonPush();

int getDigit(int number, int index);
void showNumber(int number, int digit);

static void redLightOn();
static void redLightOff();
static void greenLightOn();
static void greenLightOff();
static void toggleLight(uint8_t lightBit);

void gateControl();
void gateStateChange(uint8_t state);
void gateStateChangeTest();

void playNote(uint8_t waveLength, uint8_t duration);
void errorTone();
void successTone();
void goodByeTone();

void numDisplayLoop(int counter);

void basicLightTest();



uint8_t numbers[] = {0b00000011, 0b10011111,0b00100101, 0b00001101, 0b10011001, 0b01001001, 0b01000001,0b00011111, 0b00000001, 0b00001001};
uint8_t stepperArray[] = {0b1000, 0b1100, 0b0100, 0b0110,0b0010, 0b0011,0b0001,0b1001};
uint8_t gateArray[] = {800,4000,1900};

int counter;
int state;
bool isFull;

int main(void)
{
	//these 3 should set everything to output if i understand correctly ddr 0 port 1
	DDRB = 0b00111010;
	DDRC = 0xFF;
	DDRD = 0b11111111; // sets the two buttons to input;
	
	SETBIT(PORTB,inButtonPin); //hopefully sets pull up resistor
	SETBIT(PORTB,outButtonPin); 
	initTimer();
	initScaler64();
	initTimer1servo();
	counter = 15;
	redLightOn();
	
	

    while (1) 
    {
	
		if (counter>0){
			isFull = false;
		//	state = open;
		}
		else if(counter <= 0 ){isFull = true;
		//if (state == open)
		//{
			//state = closed;
		//}
		}
		/*	
		
		OCR1A = PULSE_MAX;
		_delay_ms(1500);	
		OCR1A = PULSE_MIN;
		_delay_ms(1500);
		*/			
		//gateStateChange(state);
	//	gateControl();
		//gateStateChange(1);
		/*
		int num = getDigit(counter, 10);
		showNumber2(num, 5);
		_delay_ms(5);
		num = getDigit(counter,1);
		showNumber2(num,4);
		_delay_ms(5) ;
		*/		
		
		
		numDisplayLoop(counter);
		inButtonPush();
		outButtonPush();
		
	// basicLightTest();
	// quarterTurnCw();
	 //gateControl(isFull);
	 
	  
	}

}

void outButtonPush(){
	if (debounce(PINB,outButtonPin))
	{
	
	if (counter < 15  )
	{
		state = open;
		gateStateChangeTest();
		goodByeTone();
		greenLightOn();
		redLightOff();
		counter ++;
		_delay_ms(1000);
		redLightOn();
		greenLightOff();
		state = closed;
		gateStateChangeTest();
		
	}
	else{errorTone();}
	}
}
void inButtonPush(){
	if (debounce(PINB, inButtonPin)) //inverted 
	{
		if (!isFull)
		{
			state = open;
			quarterTurnCw();
			//gateStateChange(1); //temp
			//gateStateChange(PULSE_MID);
			gateStateChangeTest();
			successTone();
			greenLightOn(); //possible logic error point
			redLightOff();
			counter --;
			/*
			uint8_t num = getDigit(counter, 10);
			 showNumber(num, 4);
			 _delay_ms(5);
			 num = getDigit(counter,1);
			 showNumber(num,5);
			 _delay_ms(5) ;
			*/ 
			
			
			//numDisplayLoop(counter);
			//TODO update  display at this point
			_delay_ms(1500);
			state = closed;
			gateStateChangeTest();
			//gateControl();
			//gateStateChange(state);
			//toggleLight(redLight);
			_delay_ms(1000);
			greenLightOff();
			redLightOn();
		}
		
		if (isFull)
		{
			errorTone();
		}
		
	}
	
	
}

void basicLightTest(void){
	
	redLightOn();
	
	for (int counter = 0; counter<10; counter++)
	{
		_delay_ms(500);
		if (counter % 2 == 0 )
		{
			toggleLight(redLight);
		}
		else if (counter %2 == 1)
		{
			toggleLight(greenLight);
		}
}
}



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

static void initTimer1servo(void){ //need to be walked thru 
	TCCR1A |= (1<< WGM11); // this and next line set up the fast pwm waveform generator
	TCCR1B |= (1<< WGM12) | (1<<WGM13);
	TCCR1B |= (1<<CS11); // 8 bit prescaling
	ICR1 = 40000; // top value = 20ms.   fcpu/presclaer/50
	TCCR1A |= (1<<COM1A1);// direct output on pb1/ oc1a
	DDRB |= (1<<1); //set pb1 to be output
	
}

void numDisplayLoop(int counter){
	
	int num = getDigit(counter,1);
	showNumber2(num,4);
	_delay_ms(5);

	
	//for (int i = 0; i<=10 && i !=4; i++ ){
		num = getDigit(counter,10);
	showNumber2(num,5);
	_delay_ms(5);
	//}
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

void showNumber(int redNum , int digit){
	PORTB &= 0b00110000; //adjustable // possible break point?// allowed lights to be uninterrupted 
	PORTD = numbers[redNum];
	SETBIT(PORTB,digit-1); // ORing portb @ bit of digit provided
	
}

void showNumber2(int redNum , int digit){
	//PORTB &= 0b00110000; //adjustable // possible break point?
	PORTC &= 0b00000011;
	PORTD = numbers[redNum];
	//SETBIT(PORTB,digit-1); // ORing portb @ bit of digit provided
	SETBIT(PORTC,digit);
}
int getDigit(int number, int index){
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

static void greenLightOn(){
	SETBIT(PORTB,greenLight);
}
void greenLightOff(){
	UNSET(PORTB,greenLight);
}
static void redLightOn(){
	SETBIT(PORTB,redLight);
}
void redLightOff(){
	UNSET(PORTB,redLight);
}

void toggleLight(uint8_t lightBit){
	TOGGLE(PORTB,lightBit);
}
void playNote(uint8_t waveLength, uint8_t duration){
	OCR2A = waveLength;
	SETBIT(DDRB,3); // change this if needed
	
	while(duration){
		_delay_ms(1);
		duration--;
	}
	UNSET(DDRB,3); 
}
void errorTone(){
	initScaler8();
	playNote(250,200);
	playNote(200, 250);	
}
void successTone(){
	initScaler256();
	playNote(200,250);
	playNote(250,300);
	playNote(300,300);
	playNote(330,200);
}
void goodByeTone(){
	initScaler64();
	playNote(200,200);
	playNote(300,250);
	
}
static void quarterTurnCw(){
	//possibly pass a value into this as j
	//PORTC &= 0b00111111;
	//this works as of 11.9
	//removed DDRC = 0b00111111 11.11
for (int j = 0; j<150;j++)
{
	for (int i = 0; i< 7;i+=2)
	{	
		//PORTC &= 0b00000011;
		PORTC = stepperArray[i];
		//PORTC &= 0b00000011; //tried to get lights to display continuously
		_delay_ms(5);
	}
}
}


void gateStateChange(uint8_t state){
	OCR1A = gateArray[state]; //stepperArray[state];
	//_delay_ms(300);
}
void gateStateChangeTest(){
	//OCR1A = gateArray[]
	
	if (state == open)
	{
		_delay_ms(500);
		OCR1A = PULSE_MAX;
	}
	if (state == closed)
	{
		_delay_ms(300);
		OCR1A = PULSE_MIN;
	}
	
}

void gateControl(){
	
	/*
	if (!isFull && state == open){
		//if not full and already open
		
		state = closed;
		_delay_ms(1000);
		gateStateChange(state);
	}
		*/
	
	
	if (counter>0){
		//if not full, the state is 1 and the gate opens
		state = open;
		gateStateChange(state);
		_delay_ms(1500);
		state = closed;
		gateStateChange(state);
	} 
	
	else {
		state = closed;
		gateStateChange(state);
		}
}



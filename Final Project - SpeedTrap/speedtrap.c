/********************************************
 *
 *  Name: Yiqian Yang
 *  Email: yiqianya@usc.edu
 *  Section: Wed 3:30pm
 *  Final Project: Speed Trap
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <stdio.h>

#include "lcd.h"
#include "encoder.h"
#include "speedtrap.h"
#include "serial.h"

#define FOSC 16000000           // Clock frequency
#define BAUD 9600               // Baud rate used
#define MYUBRR (FOSC/16/BAUD-1) // Value for UBRR0 register






int main(void) {

    // Initialize the LCD, timer, encoder, and serial modules
	lcd_init();
	init_timer();
	init_encoder();
	init_serial(MYUBRR);
	// If you want to use polling instead, comment the line below, and uncomment code for polling in while loop
	init_sensorinterrupt();
	
    // Enable interrupts
	sei();
	
	// Enable transistor 
	DDRB &= ~((1 << PB3) | (1 << PB4));
	
	// Enable LED
	DDRB |= (1 << PB5);
	
	// Enable Buzzer
	DDRD |= (1 << PD3);
	
	// Enable tri-state buffer
	// use bit PD2 to control the tri-state buffer
	DDRD |= (1 << PD2);
	PORTD &= ~(1 << PD2);
	
    // Show the splash screen
	lcd_writecommand(1);
	lcd_moveto(0,1);
	char* fp = "Final Project";
	lcd_stringout(fp);
	lcd_moveto(1,2);
	char* name = "Yiqian Yang";
	lcd_stringout(name);
	_delay_ms(1500);
	lcd_writecommand(1);
	
	// Get maxspeed from last run
	t = eeprom_read_byte((void *) 312);
	if((t > 0) && (t < 100))
		maxspeed = eeprom_read_byte((void *) 312); 
	else // set a default count
		maxspeed = 50; 
		
	char tspd[17];
	snprintf(tspd, 17, "%d", maxspeed);
	lcd_moveto(1,0);
	char* m = "Max:";
	lcd_stringout(m);
	lcd_stringout(tspd);
	
	
    while (1) {                 // Loop forever	
	
		
//	Polling sensor method also works
/*
		// Left sensor
		if((PINB & (1 << PB3)) == 0) {
			lcd_writecommand(1);
			// char* name = "Stop";
			// lcd_stringout(name);
			PORTB |= (1 << PB5); 		
			start_timer();
			testspeed = 0;
			hasStarted = 1;
		}
		
		// Right sensor
		if((PINB & (1 << PB4)) == 0) {
			if(hasStarted){
				PORTB &= ~(1 << PB5);
				stop_timer();
			}
			hasStarted = 0;
		}
*/		
		// NORMAL SPEED PRINTING
		if(!isTooSlow && isOK){
			answer = TCNT1;
			lcd_writecommand(1);
			
			// This prints time in ms
			// some arithmetics w/o using floats, can also use /10 and %10
			lcd_moveto(0,0);
			unsigned int aa = answer % 125;
			answer = answer - aa;
			unsigned int bb = answer / 125;
			aa = aa * 10;
			unsigned int cc = aa % 125;
			aa = aa - cc;
			aa = aa / 125;
			aa = aa * 8;
			unsigned int dd = aa % 10;
			aa = aa - dd;
			aa = aa / 10;
			bb = bb * 8;
			bb = bb + aa;
			char msc[17];
			snprintf(msc, 17,"%u",bb);
			lcd_stringout(msc);
			char* ms = "ms";
			lcd_stringout(ms);
					
			// This prints speed in cm/sec
			unsigned int aaa = DIST % bb;
			unsigned int bbb = DIST - aaa;
			bbb = bbb / bb;
			aaa = aaa * 10;
			unsigned int ccc = aaa % bb;
			aaa = aaa - ccc;
			aaa = aaa / bb;
			
			lcd_moveto(0,8);
			char* me = "Me:";
			lcd_stringout(me);
			
			unsigned char spd[17];
			snprintf(spd, 17,"%u.%u",bbb,aaa);
			lcd_moveto(0,11);
			lcd_stringout(spd);
			
			// Set speed
			testspeed = bbb;
			mmspeed = bbb*10+aaa;
			
			// Reset flag
			isOK = 0;
			leftsensorflag = 0;
			PCMSK0 |= (1 << PCINT3);
		}
		
		// CHECKS IF 4 SECONDS HAS PASSED
		if (isTooSlow) {
			PORTB &= ~(1 << PB5);
			stop_timer();
			isOK = 0;
			lcd_writecommand(1);
			lcd_moveto(0,4);
			char* slow = "Too Slow";
			lcd_stringout(slow);
			isComplete = 0;
			leftsensorflag = 0;
			
			// Reset sensor interrupts
			PCMSK0 |= (1 << PCINT3);
			PCMSK0 &= ~(1 << PCINT4);
		}
		
		// ROTARY ENCODER
		if (changed){
			char maxspd[17];
			snprintf(maxspd, 17, "%d", maxspeed);
			eeprom_update_byte((void *) 312, maxspeed); 
			lcd_moveto(1,4);
			lcd_stringout(maxspd);
			changed  = 0;
		}
		
		// SERIAL RECEIVING
		if(isValid){
			int receive;
			sscanf(data, "%d", &receive); 
			char received[10];
			snprintf(received, 10, "%d.%d", receive/10, receive%10);
			lcd_moveto(1,0);
			// lcd_stringout(data);
			lcd_stringout(received);
			
			if(maxspeed < receive/10 && buzzerOn){
				//buzzer();		// delay buzzer
				start_buzzer();				
			}
			
			// Reset isValid flag
			isValid = 0;
		}
		
		// SERIAL SENDING
		if(isComplete){
			// This prints speed in mm/sec
			char mmsec[10];
			snprintf(mmsec, 10, "%d", mmspeed);
			lcd_moveto(1,10);
			//lcd_stringout(mmsec);
			
			// This set TX buffer to <mmspeed>
			char send[10];
			snprintf(send, 10, "<%d>", mmspeed);
			serial_stringout(send);
			
			isComplete = 0;
		}
		
		// STOP BUZZER
		if(buzzCounter>500){
			TCCR0B &= ~(1 << CS12);
			stop_buzzer();				
		}
	}
}

void init_sensorinterrupt(){
	// Enable interupt
	PCICR |= (1 << PCIE0);
	// For PORTB3
	PCMSK0 |= (1 << PCINT3);
	//state = 0;
}

ISR(PCINT0_vect){
	if(leftsensorflag == 0){
		leftsensorflag = 1;
		// FOR PORTB3
		PCMSK0 &= ~(1 << PCINT3);
		//start_timer();
		// TURN ON LED FOR TIMING
		//PORTB |= (1 << PB5);
		//state = 1;
		lcd_writecommand(1);
		PORTB |= (1 << PB5); 		
		start_timer();
		testspeed = 0;
		hasStarted = 1;
		
		// FOR PORTB4
		PCMSK0 |= (1 << PCINT4);		
		
	}
	else {
		PCMSK0 &= ~(1 << PCINT4);
		if(hasStarted){
			PORTB &= ~(1 << PB5);
			stop_timer();
		}
		hasStarted = 0;
	}
}

void start_timer() {
	// TURN ON TIMER
	TCCR1B |= (1 << CS10) | (1 << CS12);	
	TCNT1 = 0;
	buzzerOn = 1;
	isComplete = 0;
}

void stop_timer() {
	TCCR1B &= ~((1 << CS10) | (1 << CS12));	
	isTooSlow = 0;
	isOK = 1;
	isComplete = 1;
}

void start_buzzer() {
	// TURN ON BUZZER
	// prescalar 128, frequency = 440 Hz, period = 2272.72 microsecond ~0.002 second
	buzzCounter = 0;
	TCCR0B |= (1 << CS12);
	buzzerOn=0;
}

void stop_buzzer() {
	// TURN OFF BUZZER
	TCCR0B &= ~(1 << CS12);
	// TURN OFF TIMER
	buzzCounter = 0;
	PORTD &= ~(1 << PD3);
}

void init_timer() {
	// TIMER1 FOR CLOCK
	isTooSlow=0;isOK = 0;
	// Set the mode for “Clear Timer on Compare”
	TCCR1B |= (1 << WGM12);
	// Enable “Output Compare A Match Interrupt”
	TIMSK1 |= (1 << OCIE1A);
	// Load the 16-bit counter modulus into OCR1A
	OCR1A = 62500;
	
	// TIMER0 FOR BUZZER
	// Set the mode for “Clear Timer on Compare”
	TCCR0A |= (1 << WGM01);
	// Enable “Output Compare A Match Interrupt”
	TIMSK0 |= (1 << OCIE0A);
	// Load the 8-bit counter modulus into OCR0A
	OCR0A = 125;

	// Reset flags
	isComplete = 0;
	hasStarted = 0;
}

ISR(TIMER1_COMPA_vect) {
	isTooSlow = 1;			
} 

ISR(TIMER0_COMPA_vect) {	
	buzzCounter++;
	// Turn PORTD3 ON and OFF
	PORTD ^= (1 << PD3);	
} 

// Delay Buzzer also works
/*
void buzzer() {
	int i, j;
	for ( j = 0; j < 3; j++) {
		for ( i = 0; i < 50; i++ ){
			PORTD |= (1 << PD3);
	        _delay_us(3401/2);
	        PORTD &= ~(1 << PD3);
	        _delay_us(3401/2);
		}
		_delay_ms(100);
	}
}
*/

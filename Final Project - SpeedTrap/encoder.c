/********************************************
 *
 *  Name: Yiqian Yang
 *  Email: yiqianya@usc.edu
 *  Section: Wed 3:30
 *  Assignment: Lab 7 - Rotary Encoder
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "encoder.h"
#include "lcd.h"

void init_encoder() {
	//lcd_init();
	// reset flag
	changed = 0;
	maxspeed = 50;
    // Initialize Rotary Encoder
	DDRC |= (1 << PC1);
	DDRC |= (1 << PC2);
	PORTC |= (1 << PC1)|(1 << PC2);
	
	// Set the PCIE1 bit in the PCICR register to enable the pin change interrupts on Port C (PCINT1).
	PCICR |= (1 << PCIE1);
	
	// Set the bits in the PCMSK1 mask register to enable interrupts for the PC1 and PC2 I/O lines. 
	PCMSK1 |= (1 << PCINT9)|(1 << PCINT10); 
	
	// Enable global interrupts.
	//sei();
	
	// Read the A and B inputs to determine the intial state 
	temp = PINC;
	a = (temp & (1 << PC2));
	b = (temp & (1 << PC1));


    if (!b && !a)
		old_state = 0;
    else if (!b && a)
		old_state = 1;
    else if (b && !a)
		old_state = 2;
    else
		old_state = 3;
	new_state = old_state;
}

ISR(PCINT1_vect){
	// Read the input bits and determine A and B
	new_state = PINC;
	a = new_state & (1<<PC2);
	b = new_state & (1<<PC1);
	
	if (!b && !a)
	new_state = 0;
    else if (!b && a)
	new_state = 1;
    else if (b && !a)
	new_state = 2;
    else
	new_state = 3;

	// For each state, examine the two input bits to see if state
	// has changed, and if so set "new_state" to the new state,
	// and adjust the maxspeed value.
	if (old_state == 0) {
		if(new_state == 1){
			maxspeed++;
		}
		if (new_state == 2){
			maxspeed--;
		}
	}
	
	else if (old_state == 1) {
		if(new_state == 0){
			maxspeed--;
		}
		if(new_state == 3){
			maxspeed++;
		}
	}
	
	else if (old_state == 2) {
		if(new_state == 3){
			maxspeed--;
		}
		if(new_state == 0){
			maxspeed++;
		}

	}
	
	else {   // old_state = 3
		if(new_state == 2){
			maxspeed++;
		}
		if(new_state == 1){
			maxspeed--;
		}
	}
	
	
	if(maxspeed >= 99)
		maxspeed = 99;
	if(maxspeed <= 0)
		maxspeed = 0;

	// If state changed, update the value of old_state,
	// and set a flag that the state has changed.
	if (new_state != old_state) {
	    changed = 1;
	    old_state = new_state;
	}
	
}

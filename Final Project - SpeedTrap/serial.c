/********************************************
 *
 *  Name: Yiqian Yang
 *  Email: yiqianya@usc.edu
 *  Section: Wed 3:30pm
 *  Assignment: Lab 9 - Serial Communications
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "serial.h"

void init_serial(unsigned short ubrr_value)
{

    // Set up USART0 registers
	UBRR0 = ubrr_value;                   // Store in the UBRR0 register     	  
	UCSR0B |= (1 << TXEN0 | 1 << RXEN0);  // Enable RX and TX 
	UCSR0C = (3 << UCSZ00);				  // Async., no parity,
										  // 1 stop bit, 8 data bits 
    // Enable interrupts
	UCSR0B |= (1 << RXCIE0);
}

void serial_txchar(char ch)
{
    while ((UCSR0A & (1<<UDRE0)) == 0){}
    UDR0 = ch;
}

void serial_stringout(char* s)
{
	//lcd_stringout(s);
	int i = 0;
	char c = s[i];
	// Call serial_txchar in loop to send a string
	while (c != '>'){
		// char what[2];
		// snprintf(what, 2, "%d", s[i]);
		// lcd_moveto(1,2);
		// lcd_stringout(what);
		serial_txchar(c);
		i++;
		c = s[i];
	}		
	serial_txchar('>');
}

ISR(USART_RX_vect)
{
	// char* what = "Hi";
	// lcd_moveto(1,2);
	// lcd_stringout(what);	
	
    // Handle received character
	char c;
	c = UDR0;	
	
	if(c == '>' && count){
		isValid = 1;
		isStarted = 0;
		data[count++] = '\0';
	}
	if (c == '<'){
		isStarted = 1;
		count = 0;
		isValid = 0;
		int i = 0;
		// for(i = 0; i < 4; i++)
			// data[i] = '\0';
	}
	else if (isStarted){
		if(count > 4){
			count = 0;
			isStarted = 0;
		}
		if((c >= '0') && (c <= '9')){
			data[count++] = c;
		}
		if (((c < '0') || (c > '9')) && c != '>'){
			count = 0;
			isStarted = 0;
		}	
	}
}

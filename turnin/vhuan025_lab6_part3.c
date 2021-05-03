/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;

unsigned long timer = 1;
unsigned long timer_count = 0;
void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	timer_count = timer;
	SREG |= 0x80;
}

void TimerOff() {
	TCCR1B = 0x00; 
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	timer_count--;
	if(timer_count == 0) {
		TimerISR();
		timer_count = timer;
	}
}

void TimerSet(unsigned long M) {
	timer = M;
	timer_count = timer;
}

enum SM_States { SM_Start, SM_Wait, SM_Release1, SM_Release0, SM_Reset  } SM_STATE;
void Button_Press() {
	unsigned char tmpA = ~PINA & 0x03; //get PA0 & PA1
	static unsigned char tmpB = 0;
	switch(SM_STATE) {
		case SM_Start:
			SM_STATE = SM_Wait;
			tmpB = 0x07;
			break;
		case SM_Wait:
			if(tmpA == 0x03)
				SM_STATE = SM_Reset;
			else if(tmpA == 0x02)
				SM_STATE = SM_Release1;
			else if(tmpA == 0x01) 
				SM_STATE = SM_Release0;
			break;
		case SM_Reset:
			if(tmpA == 0x02)
				SM_STATE = SM_Release1;
			else if(tmpA == 0x01)
				SM_STATE = SM_Release0;
			else
				SM_STATE = SM_Reset;
			break;
		case SM_Release0:
			if(tmpA == 0x00)
                                SM_STATE = SM_Wait;
                        else if(tmpA == 0x03)
                                SM_STATE = SM_Reset;
			else if(tmpA == 0x02)
				SM_STATE = SM_Release1;
			else
				SM_STATE = SM_Release0;
                        break;
		case SM_Release1:
			if(tmpA == 0x00) 
                                SM_STATE = SM_Wait;
                        else if(tmpA == 0x03)
                                SM_STATE = SM_Reset;
			else if(tmpA == 0x01) 
				SM_STATE = SM_Release0;
                        else
                                SM_STATE = SM_Release1;
                        break;
		default:
			SM_STATE = SM_Wait;
			break;
	}
	switch(SM_STATE) {
		case SM_Release1:
			if(tmpB > 0)
				--tmpB;
			break;
		case SM_Release0:
			if(tmpB < 9)
				++tmpB;
			break;
		case SM_Reset:
			tmpB = 0;
			break;
		default:
			break;
	}
	PORTB = tmpB;
}


int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(100);
	TimerOn();
	SM_STATE = SM_Start;
    /* Insert your solution below */
    while (1) {
	    Button_Press();
	    while(!TimerFlag);
	    TimerFlag = 0;
    }
    return 1;
}

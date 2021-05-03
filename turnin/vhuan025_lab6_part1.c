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

/*
enum BL_States { BL_Start, BL_Off, BL_On } BL_State;
void Tick_BlinkLED() {
	switch(BL_State) {
		case BL_Start:
			BL_State = BL_Off;
			break;
		case BL_Off:
			BL_State = BL_On;
			break;
		case BL_On:
			BL_State = BL_Off;
			break;
		default:
			BL_State = BL_Start;
			break;

	}

	switch(BL_State) {
		case BL_Off:
			PORTB = 0x00;
			break;
		case BL_On:
			PORTB = 0x01;
			break;
		default:
			break;

	}
}*/

enum TL_States { TL_Start, TL_T0, TL_T1, TL_T2 } TL_State;
void Tick_ThreeLED() {
	switch(TL_State) {
                case TL_Start:
                        TL_State = TL_T0;
                        break;
                case TL_T0:
                        TL_State = TL_T1;
                        break;
                case TL_T1:
                        TL_State = TL_T2;
                        break;
		case TL_T2:
			TL_State = TL_T0;
			break;
                default:
                        TL_State = TL_Start;
                        break;

        }

        switch(TL_State) {
                case TL_T0:
                        PORTB = 0x01;
                        break;
                case TL_T1:
                        PORTB = 0x02;
                        break;
		case TL_T2:
			PORTB = 0x04;
			break;
                default:
                        break;

        }
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(1000);
	TimerOn();
	//BL_State = BL_Start;
	TL_State = TL_Start;
    /* Insert your solution below */
    while (1) {
	    //Tick_BlinkLED();
	    Tick_ThreeLED();
	    while(!TimerFlag);
	    TimerFlag = 0;
    }
    return 1;
}

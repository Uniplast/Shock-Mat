/*
 * main.c
 *
 * Created: 12/19/2020 11:58:34 PM
 *  Author: unipl
 */ 


/*This code is written for the ATTiny85, using the internal 8MHz clock, and NO clock divider.*/

#include <xc.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint16_t timerCounter = 0;

int main(void) {
	//Disable global interrupts
	di();
	
	//Set IO pin modes
	DDRB = 0;
	DDRB |= (1 << PB3);

	PORTB = 0;

	//Reset Timer1
	TCCR1 = 0;
	TCNT1 = 0;

	//Clock Freq: 8000000Hz, Prescaler: 32, Timer Freq: 2000Hz
	/*(Clock Freq / (timer freq * prescaler)) - 1
	This calculation isn't 100% accurate because of the
	added overhead from the rest of the code. In this case,
	I found 15 to give a 1KHz signal.*/
	OCR1C = 15;
	
	//Set 256 Prescaler and CTC Mode
	TCCR1 |= (1 << CTC1) | (1 << CS12) | (1 << CS11);

	//Shut off Timer0
	//PRR &= ~(1 << PRTIM0);

	//Enable timer interrupts
	TIMSK |= (1 << OCIE1A);

	//Enable global interrupts
	ei();
	
    while(1) {
		if (timerCounter >= 25000) {
			di();
			PORTB &= ~(1 << PB3);
			timerCounter = 0;
			_delay_ms(15000);
			ei();
		}
    }
}

ISR(TIMER1_COMPA_vect) {
	PORTB ^= (1 << PB3);
	timerCounter++;
}
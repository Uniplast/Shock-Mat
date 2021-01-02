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

	//Set all pins low
	PORTB = 0;

	//Reset Timer1
	TCCR1 = 0;
	TCNT1 = 0;

	//Clock Freq: 8000000Hz, Prescaler: 32, Timer Freq: ?
	/*(Clock Freq / (timer freq * prescaler)) - 1
	This calculation isn't 100% accurate because of the
	added overhead from the rest of the code. In this case,
	I found 15 to give a ?KHz signal.*/
	OCR1C = 15;
	
	//Set 32 Prescaler and CTC Mode
	TCCR1 |= (1 << CTC1) | (1 << CS12) | (1 << CS11);

	//Shut off Timer0
	//Timer0 is actually needed for the _delay_ms function
	//PRR &= ~(1 << PRTIM0);

	//Enable timer interrupts
	TIMSK |= (1 << OCIE1A);

	//Enable global interrupts
	ei();
	
    while(1) {
		if (timerCounter >= 25000) {
			di();
			PORTB &= ~(1 << PB3); //Make sure PB3 is LOW so the N-Mosfet doesn't burn up from constantly conducting current.
			timerCounter = 0;
			_delay_ms(15000);
			ei();
		}
    }
}

ISR(TIMER1_COMPA_vect) {
	//Alternate PB3 states every time this interrupt is triggered
	PORTB ^= (1 << PB3);
	
	//Increment timer counter
	timerCounter++;
}
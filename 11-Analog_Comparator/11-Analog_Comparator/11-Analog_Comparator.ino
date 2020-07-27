/*
* Learning AVR with Arduino
* Lesson: Analog Comparator
* Author: Morgan
* Date: 24/July/2020
* IDE: Arduino 1.8.13
* 
* Analog Comparator Pin-Out:
* Negative input : AIN1 / PORTD7 / Arduino digital_pin 7
* Positive input : AIN0 / PORTD6 / Arduino digital_pin 6 , Reference pin V+
* 
*/

#include<avr/io.h>
#include<avr/interrupt.h>
#define F_CPU 16000000UL

ISR(ANALOG_COMP_vect){
  if(ACSR & (1<<ACO)){
    // check comparator status of output
    // comparator positive output indicate reference voltage greater than sample voltage.
	// V+ > V-
    PORTC &= ~(1<<PC0);        // turn off LED at PORTC0
  }
  else{
    // comparator negative output indicate sample voltage greater than reference voltage.
	// V- > V+
    PORTC |= (1<<PC0);       // turn on LED at PORTC0
  }
}

void comparator_setup(){
  ADCSRB &= ~(1<<ACME);        // set AIN1/PORTD7 to negative input
  ACSR = 0;                    // clear ACSR register and set interrupt mode as Toggle
  ACSR |= (1<<ACIE);           // enable analog comparator interrupt
  sei();                       // enable global interrupt
}

void pin_setup(){
  DDRC |= (1<<PC0);            // set PORTC0 as output
  PORTC &= ~(1<<PC0);          // set PORTC0 as LOW
}

int main(){
  pin_setup();
  comparator_setup();
  while(1){};
}

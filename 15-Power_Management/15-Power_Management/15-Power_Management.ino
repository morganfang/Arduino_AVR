/*
* Learning AVR with Arduino
* Lesson: Power Management, Sleep
* Author: Morgan
* Date: 12/August/2020
* IDE: Arduino 1.8.13
*
* press button switch to wake cpu up.
* 
*/

#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#define F_CPU 16000000UL

#define sleep() __asm__ __volatile__ ("sleep")    // sleep CPU

void power_down_setup(){
  SMCR = 0;                         // clear SMCR
  SMCR = (1<<SM1);                  // set Power_Down mode
  SMCR |= (1<<SE);                  // enable sleep
}

void BOD_disable(){
  MCUCR = (1<<BODS)|(1<<BODSE);     // both BODS and BODSE must be set to 1 before change BOD setting
  MCUCR = (1<<BODSE);               // disable BOD
}

void ADC_disable(){
  ADCSRA = 0;                       // disable ADC convert
  ACSR = (1<<ACD);                  // disable comparator
}

void ext_interrupt0_setup(){
  cli();                            // disable interrupt
  EICRA = 0;                        // Low level if INT0 generates an interrupt request
  EIMSK = (1<<INT0);                // external interrupt request 0 enable
  sei();                            // enable interrupt
}

void power_reduction_all(){
  PRR = 0xFF;                       // power reduction
}

void pin_setup(){
  DDRD &= ~(1<<PD2);                // set PORTD2/INT0 as input
  DDRD |= (1<<PD3);                 // set PORTD3 as output
  PORTD &= ~(1<<PD3);               // set PORTD3 as Low
}

ISR(INT0_vect){
  SMCR &= ~(1<<SE);                 // disable sleep
}

int main(){
  pin_setup();
  ext_interrupt0_setup();

  while(1){
    PORTD |= (1<<PD3);              // turn on LED
    _delay_ms(200);                 // delay 200 ms
    cli();                          // disable interrupt
    ADC_disable();                  // disable ADC convert
    power_reduction_all();          // power reduction
    power_down_setup();             // set power down mode
    BOD_disable();                  // disable BOD
    PORTD &= ~(1<<PD3);             // turn off LED
    sei();                          // enable interrupt
    sleep();                        // CPU sleep
  }
}

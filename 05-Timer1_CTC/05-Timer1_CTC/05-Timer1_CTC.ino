/*
* Learning AVR with Arduino
* Lesson: Timer1 CTC Mode
* Author: Morgan
* Date: 13/July/2020
* IDE: Arduino 1.8.13
* 
* calculate:
* 1. frequency = I/O clock frequency / ( prescaler * ( 1+OCR1A ) )  [Hz]
* 2. duty cycle = 50 %
*/


#include<avr/io.h>

#define F_CPU 16000000UL

void timer1_ctc_setup(){
  TCCR1A = 0;             // clear TCCR1A
  TCCR1B = 0;             // clear TCCR1B
  TCNT1 = 0;              // count from 0

  // For CTC Mode, compare output mode should set as Toggle.
  // COM1A1/COM1B1 = 0, COM1A0/COM1B0 = 1.
  TCCR1A |= (1<<COM1A0)|(1<<COM1B0);       // set wave output as toggle
  
  // For Timer1 CTC Mode, WGM[13:10]=0100
  // only WGM12 set to 1.
  TCCR1B |= (1<<WGM12);                   // set CTC mode.

  // Prescaler 256, CS[12:10]= 100
  TCCR1B |= (1<<CS12);                    // system frequency 16 MHz/256 = 62500 Hz

  OCR1A = 62500;                          // 62500/62500 = 1 Hz
  OCR1B = 31250;                          // the same frequency as OCR1A, just generate wave earlier.
  
}

void pin_setup(){
  DDRB |= (1<<PB1)|(1<<PB2);              // set PORTB1, PORTB2 as output
}

int main(){
  timer1_ctc_setup();
  pin_setup();
  while(1){};
}

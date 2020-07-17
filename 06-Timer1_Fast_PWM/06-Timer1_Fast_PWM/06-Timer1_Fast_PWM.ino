/*
* Learning AVR with Arduino
* Lesson: Timer1 Fast PWM Mode
* Author: Morgan
* Date: 16/July/2020
* IDE: Arduino 1.8.13
* 
* calculate:
* 1. frequency = I/O clock frequency / ( prescaler * MAX )  [Hz]
*    For 8-bit, MAX = 256. For 16-bit, MAX = 65536
* 2. duty cycle = ( 1 + OCR1n )/ MAX  %
*     n: A or B
*/


#include<avr/io.h>
#define F_CPU 16000000UL

void timer1_fast_pwm(){
  TCCR1A = 0;           // clear TCCR1A
  TCCR1B = 0;           // cleaer TCCR1B
  TCNT1 = 0;            // clear TCNT1

  // set OC1A as High on compare match. COM1A1/COM1A0 = 11
  // set OC1B as Low on compare match. COM1B1/COM1B0 = 10
  TCCR1A |= (1<<COM1A1)|(1<<COM1B1);

  // Timer1 Fast PWM Mode 8-bit resilution WGM[13:10]=0101
  TCCR1A |= (1<<WGM10);
  TCCR1B |= (1<<WGM12);

  // prescaler 64, CS[12:10]=011
  TCCR1B |= (1<<CS11)|(1<<CS10);

  // frequency = I/O clock frequency / ( prescaler * 256 ) [Hz]
  // Duty cycle = ( 1+ OCR1n ) / 256 % [n:A or B]
  OCR1A = 127;        // duty cycle = ( 1 + 127 )/256 = 50%
  OCR1B = 64;         // duty cycle = ( 1 + 64 )/256 = 25.4%
}

void pin_setup(){
  DDRB |= (1<<PB1)|(1<<PB2);         // set OC1A/PORTB1 and OC1B/PORTB2 as output
  PORTB &= ~(1<<PB1)|~(1<<PB2);      // set OC1A/PORTB1 and OC1B/PORTB2 as LOW
}

int main(){
  timer1_fast_pwm();
  pin_setup();
  while(1){}
}

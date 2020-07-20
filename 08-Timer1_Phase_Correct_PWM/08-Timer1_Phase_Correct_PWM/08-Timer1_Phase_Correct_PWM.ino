/*
* Learning AVR with Arduino
* Lesson: Timer1 Phase Correct PWM Mode
* Author: Morgan
* Date: 18/July/2020
* IDE: Arduino 1.8.13
* 
* Calculation:
* 1. frequency = f_clk_io / ( 2 * prescaler_value * MAX ) [Hz]
* 2. duty cycle = OCR1n / MAX
*     
*/

#include<avr/io.h>
#define F_CPU 16000000UL

void pin_setup(){
  DDRB |= (1<<PB1)|(1<<PB2);          // set OC1A/PORTB1 and OC1B/PORTB2 as output
  PORTB &= ~(1<<PB1)|~(1<<PB2);       // set OC1A/PORTB1 and OC1B/PORTB2 as LOW
}

void timer1_phase_correct_PWM_mode(){
  TCCR1A = 0;                 // clear TCCR1A
  TCCR1B = 0;                 // clear TCCR1B
  TCNT1 = 0;                  // clear TCNT1
  // set compare output mode as "Clear/non-inverting mode" [COM1A1/COM1B1][COM1A0/COM1B0] = 10
  // set phase correct PWM, 8-bit mode WGM[13:10]=0001
  // prescaller 8 CS[12:10]=010
  
  TCCR1A |= (1<<COM1A1)|(1<<COM1B1)|(1<<WGM10);
  TCCR1B |= (1<<CS11);

  // frequency = 16 MHz / (2 * 8 * 255) = 3.9 KHz
  // duty cycle for OC1A: 127 / 255 = 49.8%
  // duy cycle for OC1B: 64 / 255 = 25.1%
  OCR1A = 127;
  OCR1B = 64;
}

int main(){
  pin_setup();
  timer1_phase_correct_PWM_mode();
  while(1){}
}

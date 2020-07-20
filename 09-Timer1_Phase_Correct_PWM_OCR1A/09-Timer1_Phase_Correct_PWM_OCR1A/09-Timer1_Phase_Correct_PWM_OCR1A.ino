/*
* Learning AVR with Arduino
* Lesson: Timer1 Phase Correct PWM OCR1A Mode
* Author: Morgan
* Date: 19/July/2020
* IDE: Arduino 1.8.13
* 
* Calculation:
* 1. frequency of OC1A = f_clk_io / ( 2 * 2 * prescaler_value * OCR1A ) [Hz]
* 2. duty cycle of OC1A = 50%
* 3. frequency of OC1A = f_clk_io / ( 2 * prescaler_value * OCR1A ) [Hz]
* 4. duty cycle = OCR1B / OCR1A %
*     
*/

#include<avr/io.h>
#define F_CPU 16000000UL

void pin_setup(){
  DDRB |= (1<<PB1)|(1<<PB2);          // set OC1A/PORTB1 and OC1B/PORTB2 as output
  PORTB &= ~(1<<PB1)|~(1<<PB2);       // set OC1A/PORTB1 and OC1B/PORTB2 as LOW
}

void timer1_phase_correct_PWM_OCR1A_mode(){
  TCCR1A = 0;                 // clear TCCR1A
  TCCR1B = 0;                 // clear TCCR1B
  TCNT1 = 0;                  // clear TCNT1

  // set [COM1A1/COM1A0] = 01  "Toggle mode"
  // set  [COM1B1/COM1B0] = 10 "Clear/non-inverting mode"
  // set phase correct PWM OCR1A , WGM[13:10]=1011
  // prescaller 1 CS[12:10]=001
  
  TCCR1A |= (1<<COM1A0)|(1<<COM1B1)|(1<<WGM11)|(1<<WGM10);
  TCCR1B |= (1<<WGM13)|(1<<CS10);

  // frequency of OC1A = 16 MHz / (2 * 2 * 2000) = 2 KHz
  // frequency of OC1B = 16 MHz / (2 * 2000) = 4 KHz
  // duty cycle of OC1A: 50 %
  // duy cycle of OC1B: 400 / 2000 = 20%
  OCR1A = 2000;
  OCR1B = 400;
}

int main(){
  pin_setup();
  timer1_phase_correct_PWM_OCR1A_mode();
  while(1){}
}

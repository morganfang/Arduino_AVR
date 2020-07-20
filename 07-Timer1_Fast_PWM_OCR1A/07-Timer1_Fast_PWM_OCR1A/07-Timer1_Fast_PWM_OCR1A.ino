/*
* Learning AVR with Arduino
* Lesson: Timer1 Fast PWM OCR1A Mode
* Author: Morgan
* Date: 17/July/2020
* IDE: Arduino 1.8.13
* 
* Compare out mode:
* 1. for COM1A1 / COM1A0 only can be setted as "Toggle" .
* 2. for COM1B1 / COM1B0 can be setted as "Clear/non-inverint mode" or "Set/inverting mode".
* 
* Calculation:
* 1. For OC1A , frequency = f_clk_io / ( 2 * prescaler_value * ( 1 + OCR1A )) [Hz]
*    Duty cycle of OC1A / PORTB1 is 50%
* 2. For OC1B , frequency = f_clk_io / ( prescaler_value * ( 1 + OCR1A )) [Hz]
*    Duty cycle of OC1B / PORTB2  is (1 + OCR2B) / (1 + OCR2A)
*     
*/


#include<avr/io.h>
#define F_CPU 16000000UL

void timer1_fast_pwm(){
  TCCR1A = 0;           // clear TCCR1A
  TCCR1B = 0;           // cleaer TCCR1B
  TCNT1 = 0;            // clear TCNT1

  TCCR1A |= (1<<COM1A0)|(1<<COM1B1);                  // OCR1A as toggle, OCR1B as clear/non-inverint mode

  // Timer1 Fast PWM OCR1A Mode resilution WGM[13:10]=1111
  TCCR1A |= (1<<WGM11)|(1<<WGM10);
  TCCR1B |= (1<<WGM13)|(1<<WGM12);

  // prescaler 8, CS[12:10]=010
  TCCR1B |= (1<<CS11);

  // OCR1 frequency = I/O clock frequency / ( 2 * prescaler * ( 1 + OCR1A ) ) [Hz]
  // OCR2 frequency = 2 * frequency of OCR1
  // Duty cycle = ( 1+ OCR1B ) / ( 1+ OCR1A ) %
  OCR1A = 999;         // duty cycle =  50%
                       // frequency = 16 MHz / ( 2 * 8 * ( 1 + 999 )) = 1K Hz
  OCR1B = 249;         // duty cycle = ( 1 + 249 )/( 1 + 999 ) = 25%
                       // frequency = 2 * 1K = 2K Hz
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

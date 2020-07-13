/*
* Learning AVR with Arduino
* Lesson: Timer1 Normal Mode
* Author: Morgan
* Date: 10/July/2020
* IDE: Arduino 1.8.13
* 
* circuit:
* (1)PB0 (digital pin 8) connect 220 ohm resistor and a LED in series to GND
* 
* caculate:
* Timer1 is 16-bit. It counts from 0 to 65535 in normal mode.
* Arduino UNO has 16 MHz crystal oscillator. It spends (65536/16) micro second in each cycle.
* After 244 rounds we can get 0.99 second.
*
* Result:
* Toggle LED in every second.
*/

#include<avr/io.h>
#include<avr/interrupt.h>
#define F_CPU 16000000UL

volatile unsigned int overflow_count = 0;

void timer1_normal_setup(){
   // set TIMER1 as Normal Mode
   // WGM[13:10] = 0
   TCCR1A &= ~(1<<WGM11)|~(1<<WGM10);
   TCCR1B &= ~(1<<WGM13)|~(1<<WGM12);
   
   // set prescaler as No prescaling
   // CS12=0, CS11=0, CS10=1
   TCCR1B &= ~(1<<CS12)|~(1<<CS11);
   TCCR1B |= (1<<CS10);

   TCNT1 = 0;             // count from 0
   TIMSK1 |= (1<<TOIE1);  // enable TIMER1 overflow interrupt
   sei();                 // enable global interrupt
}

void pin_setup(){
  DDRB |= (1<<PB0);      // set PORTB0 as output
  PORTB &= ~(1<<PB0);    // set PORTB0 as LOW
}

ISR(TIMER1_OVF_vect){
  overflow_count ++;
  
  if(overflow_count == 244){
    PORTB ^= (1<<PB0);     // toggle PORTB0
    overflow_count = 0;
  }
}
int main(){
  timer1_normal_setup();
  pin_setup();
  while(1){};
}

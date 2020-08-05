/*
* Learning AVR with Arduino
* Lesson: Timer1 Input Capture
* Author: Morgan
* Date: 22/July/2020
* IDE: Arduino 1.8.13
* 
*  ___________________________________
* |              PIN_OUT              |
* |   Atmega328P    |   Arduino UNO   |
* |-----------------|-----------------|
* |  PORTB3 / OC2A  |  digital pin 11 |
* |  PORTD3 / OC2B  |  digital pin 3  |
* |  PORTB0 / ICP1  |  digital pin 8  |
* -------------------------------------
*
* Generate wave with 8-bit Timer2 for test:
* Fast PWM Mode
* 1. frequency = I/O clock frequency / ( prescaler * 256 )  [Hz]
* 2. duty cycle = ( 1 + OCR2n ) / 256  %
*     n: A or B
*     
* Timer1 Input Capture to measure frequency and duty cycle of external wave.
* 
*       ________________          ____________
*      |                |        |
*    __| positive cycle |________| 
*      ^                ^        ^
*      1                2        3
*      
* To measure period, we can get time-stamp1 and time-stamp3 at rising edge respectively.
* To measure duty cycle, (time-stamp2 - time-stamp1) / (time-stamp3 - time-stamp1) * 100%
*    
*/


#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#define F_CPU 16000000UL

void timer2_fast_pwm(){
  TCCR2A = 0;           // clear TCCR1A
  TCCR2B = 0;           // cleaer TCCR1B
  TCNT2 = 0;            // clear TCNT1

  // set OC1A/B as non-inverting mode, clear on compare match. COM1A1/COM1A0 = 10
  TCCR2A |= (1<<COM2A1)|(1<<COM2B1);

  // Timer1 Fast PWM Mode WGM[22:20]=011
  TCCR2A |= (1<<WGM21)|(1<<WGM20);

  // prescaler 64, CS[22:20]=100
  TCCR2B |= (1<<CS22);

  // frequency = 16 M / ( 64 * 256 ) = 976.56 [Hz]
  // OC2A/PORTB3 Duty cycle = ( 1+ 64 ) / 256 = 25.4 %
  // OC2B/PORTD3 Duty cycle = ( 1+ 192 ) / 256 = 75.4 %
  OCR2A = 64;
  OCR2B = 192;
}

volatile unsigned int last_time = 0, current_time = 0;      // store time-stamp
volatile unsigned long period = 0,  positive_cycle_width = 0;
volatile boolean capture_flag = 0;                          // set 1, capture positive cycle finished
volatile unsigned int overflow_cnt = 0;                     // count overflow


ISR(TIMER1_CAPT_vect){
 if(TCCR1B & (1<<ICES1)){
  // rising edge
  overflow_cnt = 0;
  last_time = current_time;
  current_time = ICR1;

   TCCR1B &= ~(1<<ICES1);                                 // set falling edge detect
   TCCR1B |= (1<<CS10);
  
  if(capture_flag){
    // second rising edge
    period =  positive_cycle_width + current_time - last_time + (overflow_cnt << 16);
    TIMSK1 &= ~(1<<ICIE1)|~(1<<TOIE1);                   // disable input capture
  }
  
 }else{
  last_time = current_time;
  current_time = ICR1;
  
   positive_cycle_width = current_time - last_time + (overflow_cnt << 16);

  TCCR1B |= (1<<ICES1)|(1<<CS10);                        // set rising edge detect
  capture_flag = 1;                                      // capture_flag set as 1 indicates positive cycle passed
  }
 
}


ISR(TIMER1_OVF_vect){
  overflow_cnt++;                    // count overflow
}

void timer1_capture(){
  TCCR1B = 0;                        // clear TCCR1B
  TCNT1 = 0;                         // clear TCNT1
  ICR1 = 0;
  TIFR1 = (1<<ICF1)|(1<<TOV1);       // clear input capture flag,timer1 overflow flag

  TCCR1B = (1<<ICES1)|(1<<CS10);     // set rising edge detect
  TIMSK1 = (1<<ICIE1)|(1<<TOIE1);    // enable input capture interrupt and timer overlow interrupt
  sei();
}

void pin_setup(){
  DDRB |= (1<<PB3);                  // set OC2A/PORTB3 as output
  DDRD |= (1<<PD3);                  // set OC2B/PORTD3 as output
  DDRB &= ~(1<<PB0);                 // set ICP1/PORTB0 as Input
  PORTB &= ~(1<<PB3);                // set OC2A/PORTB3 as LOW
  PORTD &= ~(1<<PD3);                // set OC2B/PORTD3 as LOW
}

int main(){
  Serial.begin(9600);
  timer2_fast_pwm();
  pin_setup();
  timer1_capture();

  while(1){
    if(capture_flag){
    _delay_ms(3000);                // delay 3 second
    
    Serial.println();
    Serial.println();

    Serial.print("positive_cycle_width = ");
    Serial.println(positive_cycle_width);
    Serial.print("Period = ");
    Serial.println(period);
    
    Serial.print("Frequency : ");
    Serial.print((F_CPU / (float)period));
    Serial.println(" Hz");
    Serial.print("Duty cycle : ");
    Serial.print(((float)positive_cycle_width / (float)period)*100);
    Serial.println(" %");
    
    capture_flag = 0;               // clear variable
    overflow_cnt = 0;               // clear variable
    timer1_capture();               // restart input capture
    }
  }
}

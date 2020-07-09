/* Learning AVR with Arduino
 *  Lesson: ext_interrupt
 *  Date: 8/July/2020
 *  Author: Morgan
 * 
 */
 
#include<avr/io.h>
#include<avr/interrupt.h>
#define F_CPU 16000000UL
volatile boolean led_state = 0;

ISR(INT0_vect){
  led_state = !led_state;         // toggle Led status
  while(!(PIND & (1<<PIND2))){}   // wait untill release button
  if (led_state){
    PORTD |= (1<<PD7);            // led_state=1, LED ON
  }else{
      PORTD &= ~(1<<PD7);         // led_state=0, LED OFF
    }
}

void setup(){
  DDRD |= (1<<PD7);                   // set PD7 as OUTPUT
  DDRD &= ~(1<<PD2);                  // set PD2 as INPUT
  PORTD |= ~(1<<PD7);                 // LED off.
  EICRA &= (~(1<<ISC01)|~(1<<ISC00)); // set INT0 LOW level interrupt request
  EIMSK |= (1<<INT0);                 // enable INT0
  sei();                              // enable interrupt
}
int main(){
  setup();
  while(1){}
}


/*
// Arduino version.
const unsigned int led_pin = 7;    //digital pin 7,PD7
const unsigned int button_pin = 2; //digital pin 2,PD2,INT0
volatile boolean led_state = LOW;  // Led initial state

void setup() {
  pinMode(led_pin, OUTPUT);        // set PD7 as output
  pinMode(button_pin, INPUT);      // set PD2 as inpput
  digitalWrite(led_pin, led_state);
  // Interrupt request when button pressed, potential of PD2 is LOW
  attachInterrupt(digitalPinToInterrupt(button_pin), toggle_led, LOW);
}

void toggle_led(){
  led_state = !led_state;                 // toggle LED
  while(digitalRead(button_pin) == LOW){} // wait untill release button
  digitalWrite(led_pin, led_state);       // output Led state
}

void loop() {
  
}
*/
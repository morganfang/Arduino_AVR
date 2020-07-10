/*
* Learning AVR with Arduino
* Lesson: Pin Change Interrupt
* Author: Morgan
* Date: 10/July/2020
* IDE: Arduino 1.8.13
* 
* circuit:
* (1)Vcc connect 10K ohm resistor and a button in series to GND
* (2)PD7 connect 220 ohm resistor and a LED in series to GND
* (3)PD2 connect between 10K ohm resistor and button
*
* Result:
* Press the button, the LED turns ON.
* Release then button, the LED turns OFF.
*/

#include<avr/io.h>
#include<avr/interrupt.h>
#define F_CPU 16000000UL
void pin_setup(){
  DDRD &= ~(1<<PD2);    // PD2,PCINT18 as input
  DDRD |= (1<<PD7);     // PD7 as output
  PORTD &= ~(1<<PD7);   // PD7 output LOW
}

void int_setup(){
  PCICR |= (1<<PCIE2);    // enable PCINT[23:16]
  PCMSK2 |= (1<<PCINT18); // enable PCINT18
  sei();                  // enable interrupt
}

ISR(PCINT2_vect){
  PORTD ^= (1<<PD7);     // toggle output PD7
}

int main(){
  pin_setup();
  int_setup();
  while(1){}
}

/*
// Arduino version.
const unsigned int led_pin = 7;    //digital pin 7,PD7
const unsigned int button_pin = 2; //digital pin 2,PD2,INT0
volatile boolean led_state = LOW;  // Led initial state
void setup(){
  pinMode(button_pin,INPUT);       // set PD7 as output
  pinMode(led_pin,OUTPUT);         // set PD2 as input
  digitalWrite(7,LOW);
  // Interrupt request when button pressed, potential of PD2 has changed
  attachInterrupt(digitalPinToInterrupt(button_pin), toggle_led, CHANGE);
}

void toggle_led(){
  led_state = !led_state;                   // toggle LED
  digitalWrite(led_pin, led_state);         // output Led state
}

void loop() {
  
}
*/

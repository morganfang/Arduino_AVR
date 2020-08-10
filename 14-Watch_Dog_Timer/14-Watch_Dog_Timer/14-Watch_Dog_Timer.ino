/*
* Learning AVR with Arduino
* Lesson: Watch Dog Timer
* Author: Morgan
* Date: 9/August/2020
* IDE: Arduino 1.8.13
*
* press button switch to reset watch_dog_timer.(feed dog)
* watch_dog_timer will trigger interrupt and then reset system after 4 seconds.
* 
*/

#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#define F_CPU 16000000UL

#define WDT_reset() __asm__ __volatile__ ("wdr")    // Rest Watch Dog Timer, feed dog

volatile unsigned int cnt = 0;      // count how many seconds passed

void ext_interrupt0_setup(){
  EICRA = 0;                        // Low level if INT0 generates an interrupt request
  EIMSK = (1<<INT0);                // external interrupt request 0 enable
  sei();                            // enable global interrupt
}

void wdt_setup(){
  cli();                                        // disable global interrupt
  WDT_reset();                                  // reset Watch Dog Timer
  WDTCSR |= (1<<WDCE)|(1<<WDE);                 // start timed sequence.
  WDTCSR = (1<<WDE)|(1<<WDIE)|(1<<WDP3);        // Timer out in 4 seconds.interrupt occur and then reset system
  sei();                                        // enable global interrupt
}

void wdt_off(){
  cli();                            // disable global interrupt
  WDT_reset();                      // reset Watch Dog Timer
  MCUSR &= ~(1<<WDRF);              // clear WDRF in MCUSR
  WDTCSR |= (1<<WDCE)|(1<<WDE);     // to set WDTCSR, [WDCE],[WDE] should set as "1" first
  WDTCSR = 0;                       // turn off Watch Dog Timer
  sei();                            // enable global interrupt
}

ISR(INT0_vect){
  WDT_reset();                      // Reset Watch Dog Timer
  cnt = 0;                          // clear counter
  Serial.println("Press button switch to Reset Watch Dog Timer !");
}

ISR(WDT_vect){
  PORTD |= (1<<PD3);                // turn on LED,Watch Dog time out interrupt occur,then reset system.
  Serial.println(" WDT_Interrupt!,system will reset!");
}

void pin_setup(){
  DDRD &= ~(1<<PD2);        // set PORTD2/INT0 as input
  DDRD |= (1<<PD3);         // set PORTD3 as output
  PORTD &= ~(1<<PD3);       // set PORTD3 as Low
}
int main(){
  Serial.begin(9600);                // Serial Monitor
  Serial.println("System start !");
  pin_setup();                       // setup pin
  ext_interrupt0_setup();            // setup external interrupt
  wdt_setup();                       // setup watch dog timer
  
  while(1){
    Serial.println((String)cnt + " seconds");
    cnt ++;
    _delay_ms(1000);                  // delay 1 second
    };
}

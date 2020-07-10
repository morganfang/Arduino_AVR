#include<avr/io.h>
#include<avr/interrupt.h>
#define F_CPU 16000000UL
void pin_setup(){
  DDRD &= ~(1<<PD2);    // PD2,PCINT18 as input
  DDRD |= (1<<PD7);     // PD7 as output
  PORTD &= ~(1<<PD7);   // PD7 output LOW
}

void int_setup(){
  //cli();                  // disable interrupt
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

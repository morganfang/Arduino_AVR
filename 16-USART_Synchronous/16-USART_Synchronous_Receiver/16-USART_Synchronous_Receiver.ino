/*
* Learning AVR with Arduino
* Lesson: USART Synchronous Mode, Receiver
* Author: Morgan
* Date: 15/August/2020
* IDE: Arduino 1.8.13
*
* send data to receiver
* 
* Arduino UNO (Transmitter) --------> Arduino UNO (Receiver)
* TxD/PD1/pin 1                       RxD/PD0/pin 0
* RxD/PD0/pin 0                       TxD/PD1/pin 1
* XCK/PD4/pin 4                       XCK/PD4/pin 4
* PB0/pin 8 connect to LED            PB0~PB3/pin 8,9,10,11 connect to LED
*/

#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

#define F_CPU 16000000UL

void USART_Syn_setup(){
  UCSR0B = (1<<RXCIE0)|(1<<RXEN0);                // enable receive complete interrupt and receive
  UCSR0C = (1<<UMSEL00)|(1<<UCSZ01)|(1<<UCSZ00);  // set synchronous mode,1 stop bit,no parity,8 bits data,
  sei();
}

unsigned char USART_receive_byte(){
  while( !(UCSR0A & (1<<RXC0)));                 // wait until data receive complete
  return UDR0;                                   // return data received
}

ISR(USART_RX_vect){
  PORTB = UDR0;                                  // read data from UDR register and turn on LED
}

int main(){
  DDRD &= ~(1<<PD4);                                  // set PORTD4/XCK as input
  DDRB |= (1<<PB3)|(1<<PB2)|(1<<PB1)|(1<<PB0);        // set PORTB0 ~ POERTB3 as output,they connect to LED
  PORTB &= ~((1<<PB3)|(1<<PB2)|(1<<PB1)|(1<<PB0));    // turn off LED
  USART_Syn_setup();
  
  while(1){}
}

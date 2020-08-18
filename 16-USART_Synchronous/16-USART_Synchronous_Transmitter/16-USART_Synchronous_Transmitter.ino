/*
* Learning AVR with Arduino
* Lesson: USART Synchronous Mode, Transmitter
* Author: Morgan
* Date: 15/August/2020
* IDE: Arduino 1.8.13
*
* send data to receiver
* 
* UBRR formula for USART synchronous mode
* UBRR = F_CPU / (2*Baud_rate) - 1
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
#define Baud_rate 9600UL                          // Baud Rate 9600 bps
#define UBRR (F_CPU/(2*Baud_rate)-1)

void USART_Syn_setup(){
  UCSR0B = 0;
  UCSR0C = 0;
  UBRR0L = UBRR;
  UBRR0H = (UBRR>>8);                              // set Baud rate as 9600 bps
  UCSR0B = (1<<TXCIE0)|(1<<TXEN0);                 // enable transmit and transmit complete interrupt
  UCSR0C = (1<<UMSEL00)|(1<<UCSZ01)|(1<<UCSZ00);   // set synchronous mode,1 stop bit,no parity,8 bits data,
  sei();
}

void USART_transmit_byte(unsigned char data){
  while( !(UCSR0A & (1<<UDRE0)));                  // wait until UDRE is empty
  UDR0 = data;                                     // push data into UDR and send to receiver
}

ISR(USART_TX_vect){
  PORTB ^= (1<<PB0);                               // toggle LED while receive complete
}

int main(){
  DDRD |= (1<<PD4);                                // set PORTD4/XCK as output
  DDRB |= (1<<PB0);                                // set PORTB0 as output,it connects to LED
  PORTB &= ~(1<<PB0);                              // turn off LED
  USART_Syn_setup();
  
  unsigned char mydata[5]={0,0x01,0x03,0x07,0x0F}; // test data
  unsigned int index = 0;                          // index for test data array
  while(1){
    for(unsigned int i=0;i<5;i++){
      USART_transmit_byte(mydata[index]);          // send data
      index++;
      _delay_ms(500);
    }
    index = 0;
    PORTB &= ~(1<<PB0);                            // turn off LED
    _delay_ms(1000);
  }
}

/*
* Learning AVR with Arduino
* Lesson: SPI Communication, Receiver
* Author: Morgan
* Date: 19/August/2020
* IDE: Arduino 1.8.13
*
* get data from transmitter,then a data to transmitter
* 
* 
* Arduino UNO (Master/Transmitter) --------> Arduino UNO (Slave/Receiver)
* SCK/PB5/pin 13                             SCK/PB5/pin 13
* MISO/PB4/pin 12                            MISO/PB4/pin 12
* MOSI/PB3/pin 11                            MOSI/PB3/pin 11
* SS/PB2/Pin 10                              SS/PB2/Pin 10
* PB0/pin 8 connect to LED                   PD4~PD7/pin 4,5,6,7 connect to LED
* PB1/pin 9 connect to LED
*/

#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

#define F_CPU 16000000UL
#define PIN_SCK  PB5
#define PIN_MISO PB4
#define PIN_MOSI PB3
#define PIN_SS   PB2

void SPI_setup(){
  SPCR = (1<<SPIE)|(1<<SPE)|(1<<SPR1)|(1<<SPR0);     // enable SPI interrupt and SPI, fosc/128
  sei();                                             // enable interrupt
}

unsigned char SPI_transmit_byte(unsigned char data){
  SPDR = data;                                      // start transmission
  while( !(SPSR & (1<<SPIF)));                      // wait for transmission complet
  //return SPDR;                                    // return data from Slave/Receiver
}

ISR(SPI_STC_vect){
  PORTD = SPDR;                                     // receive data while tramsmission complete
  SPI_transmit_byte('1');                           // send '1' to master/transmitter
}

void pin_setup(){
  DDRD |= (1<<PD4)|(1<<PD5)|(1<<PD6)|(1<<PD7);         // set PORTD4 ~ PORTD7 as output
  DDRB &= ~((1<<PIN_SCK)|(1<<PIN_MOSI)|(1<<PIN_SS));   // set pin SCK, MOSI, SS as output
  DDRB |= (1<<PIN_MISO);                               // set pin MISO as input
  PORTD &= ~((1<<PD4)|(1<<PD5)|(1<<PD6)|(1<<PD7));     // turn off LED at PORTD4 ~ PORTD7
}

int main(){
  pin_setup();                                       // setup I/O pin
  SPI_setup();                                       // initial SPI
  
  while(1){}
}

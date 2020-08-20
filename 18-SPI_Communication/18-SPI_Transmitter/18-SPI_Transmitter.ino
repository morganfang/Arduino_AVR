/*
* Learning AVR with Arduino
* Lesson: SPI Communication, Transmitter
* Author: Morgan
* Date: 19/August/2020
* IDE: Arduino 1.8.13
*
* send data to receiver and get a data from receiver
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
  SPCR = (1<<SPIE)|(1<<SPE)|(1<<MSTR)|(1<<SPR1)|(1<<SPR0); // enable SPI interrupt and SPI,set master mode,fosc / 128
  sei();                                                   // enable interrupt
}

unsigned char SPI_transmit_byte(unsigned char data){
  SPDR = data;                                     // start transmission
  while( !(SPSR & (1<<SPIF)))                      // wait for transmission complet
  return SPDR;                                     // return data from Slave/Receiver
}

ISR(SPI_STC_vect){
  PORTB ^= (1<<PB0);                               // toggle LED while transmission/receive complete
}

void pin_setup(){
  DDRB |= (1<<PB0)|(1<<PB1);                        // set PORTB0, PORTB1 as output
  DDRB |= (1<<PIN_SCK)|(1<<PIN_MOSI)|(1<<PIN_SS);   // set pin SCK, MOSI, SS as output
  DDRB &= ~(1<<PIN_MISO);                           // set pin MISO as input
  PORTB &= ~((1<<PB0)|(1<<PB1));                    // turn off LEDs
}

int main(){
  pin_setup();                                       // setup I/O pin
  SPI_setup();                                       // initial SPI
  
  unsigned char mydata[5]={0,0x10,0x30,0x70,0xF0};   // test data
  unsigned int index = 0;                            // index for test data array
  unsigned char received_data;                       // store data from Slave/receiver
  
  while(1){
    PORTB &= ~(1<<PIN_SS);                               // set SS as Low before transmission
    for(unsigned int i=0;i<5;i++){
      received_data = SPI_transmit_byte(mydata[index]);  // send data and receive data
      index++;
      _delay_ms(500);
    }
    index = 0;
    PORTB &= ~(1<<PB0);                               // turn off LED at PORTB0
    PORTB |= (1<<PIN_SS);                             // set SS as High, stop transmission data to receiver
    if(received_data = '1') PORTB |= (1<<PB1);        // turn on LED at PORTB1
    _delay_ms(1000);                                  // delay 1 second
    PORTB &= ~(1<<PB1);                               // turn off LED at PORTB1
  }
}

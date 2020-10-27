/*
* Learning AVR with Arduino
* Lesson: TWI Communication, Master Transmitter Slave Receiver Mode
* Author: Morgan
* Date: 19/August/2020
* IDE: Arduino 1.8.13
*
* Slave Receiver
*  
* Arduino UNO (Master/Transmitter) --------> Arduino UNO (Slave/Receiver)
* SCL/PC5/pin A5                             SCL/PC5/pin A5
* SDA/PC4/pin A4                             SDA/PC4/pin A4
* PB0/pin 8 connect to LED                   PD4~PD7/pin 4,5,6,7 connect to LED
*/

#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#define F_CPU 16000000UL

#define Slave_Addr 0x20;               // Slave Address

// Status Code for Slave Receiver
#define SR_SLA_W            0x60       // own SLA+W has been received, ACK has been returned
#define SR_SLA_W_DATA_ACK   0x80       // previously addressed with own SLA+W, data has been received and ACK has been returned
#define SR_SLA_W_DATA_NACK  0x88       // previously addressed with own SLA+W, data has been received and NOT ACK has been returned
#define SR_STOP_RESTART     0xA0       // A STOP condition or repeated START condition has been received while addressed as Slave


volatile unsigned int index = 0;       // index of data array
unsigned char data_received[5];        // store data from Master

void TWI_setup(){
  TWAR = Slave_Addr;                                  // set Address of Slave
  TWBR = 72;
  TWSR &= ~((1<<TWPS1)|(1<<TWPS0));                   // TWBR = 72,and TWSP[1:0]= 0 for setting bit rate as 100K bps
  TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA)|(1<<TWIE);    // enable TWI and interrupt
  sei();
}

ISR(TWI_vect){
  switch(TWSR & 0xF8){
    case SR_SLA_W:
      break;
      
    case SR_SLA_W_DATA_ACK:
      data_received[index] = TWDR;                      // read and store data
      index++;
      if(index > 4) index = 0;
      break;
      
    case SR_STOP_RESTART:
      break;

  }// switch segment
  TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA)|
         (0<<TWSTA)|(0<<TWSTO)|(0<<TWWC);
}// ISR segment

void pin_setup(){
  DDRD |= (1<<PD4)|(1<<PD5)|(1<<PD6)|(1<<PD7);          // four LED connected to PORTD7 ~ PORTD4
  PORTD &= ~((1<<PD4)|(1<<PD5)|(1<<PD6)|(1<<PD7));      // turn off LEDs
}

int main(){
  pin_setup();                                         // setup I/O pin
  TWI_setup();                                         // initial TWI
  
  while(1){
    for(unsigned int i=0;i<5;i++){                     // show the data from Master
      PORTD = data_received[i];
      _delay_ms(500);
    }
    
  }//while segment
}// int main segment

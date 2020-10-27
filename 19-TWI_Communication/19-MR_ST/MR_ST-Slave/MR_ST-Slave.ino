/*
* Learning AVR with Arduino
* Lesson: TWI Communication, Master Receiver Slave Transmitter Mode
* Author: Morgan
* Date: 20/October/2020
* IDE: Arduino 1.8.13
*
* Slave Transmitter
*
* Arduino UNO (Master/Receiver) ------------ Arduino UNO (Slave/Transmitter)
* SCL/PC5/pin A5                             SCL/PC5/pin A5
* SDA/PC4/pin A4                             SDA/PC4/pin A4
* PB0/pin 8 connect to LED                   
*/

#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#define F_CPU 16000000UL

#define Slave_Addr 0x20;                // Slave Address

// Status Code for Slave Transmitter
#define ST_SLA_R             0xA8       // own SLA+R has been received and ACK has been returned
#define ST_ARBITRATION_LOST  0xB0       // arbitration lost in SLA+R/W as Master. own SLA_R has been received and ACK has been returned
#define ST_DATA_ACK          0xB8       // data byte in TWDR has been transmitted and ACK has been received
#define ST_DATA_NACK         0xC0       // data byte in TWDR has been transmitted and NOT ACK has been received
#define ST_LAST_DATA_ACK     0xC8       // last data byte in TWDR has been transmitted and ACK has received


volatile unsigned int index = 0;
unsigned char data_to_master[5]="HELLO";


void TWI_send_byte(unsigned char data){
  TWDR = data;                                        // put data into TWDR
  TWSR = (1<<TWEN)|(1<<TWINT);
  while(!(TWCR & (1<<TWINT)));                        // wait for TWINT flag set
}

void TWI_setup(){
  TWAR = Slave_Addr;
  TWBR = 72;
  TWSR &= ~((1<<TWPS1)|(1<<TWPS0));                   // TWBR = 72,and TWSP[1:0]= 0 for setting bit rate as 100K bps
  TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA)|(1<<TWIE);    // enable TWI and interrupt
  sei();                                              // enable global interrupt
}

ISR(TWI_vect){

  switch(TWSR & 0xF8){
    case ST_SLA_R:
      TWI_send_byte(data_to_master[index]);			     // send first data byte to Master
	    index ++;
      break;

    case ST_DATA_ACK:								                 // ACK from Master has been received, send data continuously
	    TWI_send_byte(data_to_master[index]);			     // send next data byte to Master
	    index ++;
	    if(index > 4) index = 0;							         // set index to zero if index greater than length of data array
      break;
	  
	  case ST_DATA_NACK:					                     // NOT ACK from Master has been received, stop tramsmission

      break;

  }// switch segment
  TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA)|
         (0<<TWSTA)|(0<<TWSTO)|(0<<TWWC);
}

int main(){
  TWI_setup();                                       // initial TWI
  
  while(1){}
}

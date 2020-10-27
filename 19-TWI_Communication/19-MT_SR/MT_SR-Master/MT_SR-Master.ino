/*
* Learning AVR with Arduino
* Lesson: TWI Communication, Master Transmitter Slave Receiver Mode
* Author: Morgan
* Date: 20/October/2020
* IDE: Arduino 1.8.13
*
* Master Transmitter
* 
* 
* Arduino UNO (Master/Transmitter) --------> Arduino UNO (Slave/Receiver)
* SCL/PC5/pin A5                             SCL/PC5/pin A5
* SDA/PC4/pin A4                             SDA/PC4/pin A4
* PB0/pin 8 connect to LED                   PD4~PD7/pin 4,5,6,7 connect to LED
*/

#include<avr/io.h>
#include<util/delay.h>
#define F_CPU 16000000UL

//  Status Code for Master Transmitter
#define MT_START            0x08           // Start condition has been transmitted
#define MT_REPEAT_START     0x10           // Repeat Start condition has been transmitted
#define MT_SLA_W_ACK        0x18           // SLA+W has been transmitted and ACK has been received
#define MT_SLA_W_NACK       0x20           // SLA+W has been transmitted and NOT ACK has been received
#define MT_DATA_ACK         0x28           // Data byte has been transmitted and ACK has been received
#define MT_DATA_NACK        0x30           // Data byte has been transmitted and NOT ACK has been received
#define MT_LOST_ARBITRATION 0x38           // Arbitration lost in SLA+W or data bytes


void TWI_START(){
  TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);       // start condition
  while(!(TWCR & (1<<TWINT)));                  // wait till complete
}

void TWI_STOP(){
  TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);       // stop condition
  while(!(TWCR & (1<<TWSTO)));                  // wait till stop condition has been sent
}

void TWI_sent_byte(unsigned char data){
  TWDR = data;                                  // put data into TWDR
  TWCR = (1<<TWINT)|(1<<TWEN);                  
  while(!(TWCR & (1<<TWINT)));                  // wait for TWINT flag set
}

void TWI_setup(){
   /*
   * System Frequency vs Bit Rate generator registor setting
   * ----------------------------------------------------------------
   * | CPU clock frequency [MHz]| TWBR | TWPS | SCL Frequency [KHz] |
   * ----------------------------------------------------------------
   * | 16                       |  12  |  0   |  400                |
   * ----------------------------------------------------------------
   * | 16                       |  72  |  0   |  100                |
   * ----------------------------------------------------------------
   */ 
  TWBR = 72;
  TWSR &= ~((1<<TWPS1)|(1<<TWPS0));  // TWBR = 72,and TWSP[1:0]= 0 for setting bit rate as 100K bps
  TWCR = (1<<TWEN);                  // enable TWI
}

void pin_setup(){
  DDRB |= (1<<PB0);					         // turn on LED at PORTB0 when TWI error occur
  PORTB &= ~(1<<PB0);
}


int main(){

  const unsigned char SLA_W = 0x20;                  // Address of Slave 0x20 + Write bit(0)
  unsigned char mydata[5]={0,0x10,0x30,0x70,0xF0};	 // data to Slave
  pin_setup();                                       // setup I/O pin
  TWI_setup();                                       // initial TWI
  
  /* send data to Slave and then STOP */
  TWI_START();										                   // Start condition
  if((TWSR & 0xF8) != MT_START){
	  // TWI error
	  PORTB |= (1<<PB0);									             // turn on LED
	  TWI_STOP();									                     // STOP TWI
  }else{
	  // START condition has been transmitted, and then send SLA+W
    TWI_sent_byte(SLA_W);
	  if((TWSR & 0xF8) != MT_SLA_W_ACK){
		// TWI error
		PORTB |= (1<<PB0);									             // turn on LED
		TWI_STOP();									                     // STOP TWI
	  }else{
		  // transmit data
		  for(unsigned int i=0;i<5;i++){
			  TWI_sent_byte(mydata[i]);				             // transmit data
			  if((TWSR & 0xF8) == MT_DATA_ACK){
				  continue;
			  }else{
				  // TWI error
				  PORTB |= (1<<PB0);									       // turn on LED
				  TWI_STOP();									               // STOP TWI
				  break;
			  }
		  }// for loop
		  TWI_STOP();								                     // data has been transmitted, STOP TWI
	  }
  }

  while(1){}
}// int main segment

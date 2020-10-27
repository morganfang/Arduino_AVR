/*
* Learning AVR with Arduino
* Lesson: TWI Communication, Master Receiver Slave Transmitter Mode
* Author: Morgan
* Date: 20/October/2020
* IDE: Arduino 1.8.13
*
* Master Receiver
*
* Arduino UNO (Master/Receiver) ------------ Arduino UNO (Slave/Transmitter)
* SCL/PC5/pin A5                             SCL/PC5/pin A5
* SDA/PC4/pin A4                             SDA/PC4/pin A4
* PB0/pin 8 connect to LED                   
*/

#include<avr/io.h>
#include<util/delay.h>
#define F_CPU 16000000UL


// Status Code for Master Receiver
#define MR_START            0x08           // Start condition has been transmitted
#define MR_REPEAT_START     0x10           // Repeat Start condition has been transmitted
#define MR_LOST_ARBITRATION 0x38           // Arbitration lost in SLA+R or NOT ACK bit
#define MR_SLA_R_ACK        0x40           // SLA+R has been transmitted and ACK has been received
#define MR_SLA_R_NACK       0x48           // SLA+R has been transmitted and NOT ACK has been received
#define MR_DATA_ACK         0x50           // DATA has been received and ACK has been returned
#define MR_DATA_NACK        0x58           // DATA has been received and NOT ACK has been returned



void TWI_START(){
  TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);       // start condition
  while(!(TWCR & (1<<TWINT)));                  // wait till complete
}

void TWI_STOP(){
  TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);       // stop condition
  while(!(TWCR & (1<<TWSTO)));                  // wait till stop condition has been sent
}

void TWI_send_byte(unsigned char data){
  TWDR = data;                                  // put data into TWDR
  TWCR = (1<<TWINT)|(1<<TWEN);                  // enable TWI
  while(!(TWCR & (1<<TWINT)));                  // wait for TWINT flag set
}

unsigned char TWI_Receive_ACK(){
  TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);        // data has been received and return ACK
  while (!(TWCR & (1<<TWINT)));
  return TWDR;
}

unsigned char TWI_Receive_NACK(){
  TWCR = (1<<TWINT)|(1<<TWEN);                  // data has been received and return NOT ACK
  while (!(TWCR & (1<<TWINT)));
  return TWDR;
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
  TWSR &= ~((1<<TWPS1)|(1<<TWPS0));             // TWBR = 72,and TWSP[1:0]= 0 for setting bit rate as 100K bps
  TWCR = (1<<TWEN);                             // enable TWI
}

void pin_setup(){
  DDRB |= (1<<PB0);					                    // turn on LED at PORTB0 when TWI error occur
  PORTB &= ~(1<<PB0);
}


int main(){
  Serial.begin(9600);
  const unsigned char addr_slave = 0x20;			 // Address of Slave
  const unsigned char SLA_R = 0x21;            // Address of Slave 0x20 + Read bit(1)
  unsigned char received_data[5];					 	   // store data from Slave
  pin_setup();                                 // setup I/O pin
  TWI_setup();                                 // initial TWI

  // Master receive data from Slave
  TWI_START();									    	         // send START condition
  if((TWSR & 0xF8) != MR_START){
	  // TWI error
	  PORTB |= (1<<PB0);							           // turn on LED at PORTB0
	  TWI_STOP();							                   // send STOP condition
  }else{
	  // status code 0x08 has been received
	  TWI_send_byte(SLA_R);			    			 	     // send address + read bit
	  if((TWSR & 0xF8) != MR_SLA_R_ACK){
		  // TWI error
		  PORTB |= (1<<PB0);							         // turn on LED at PORTB0
		  TWI_STOP();							                 // send STOP condition
	  }else{
		  // status code 0x40 has been received, Slave device will start to send data
		  for(unsigned int i=0;i<5;i++){
			  // store data from Slave
			  if(i<4){
				  received_data[i] = TWI_Receive_ACK();	  // store data in data array and return ACK
				  if((TWSR & 0xF8) != MR_DATA_ACK){
					  // TWI error
					  PORTB |= (1<<PB0);				            // turn on LED at PORTB0
					  TWI_STOP();						                // send STOP condition
					  break;
				  }
			  }else{
				  received_data[i] = TWI_Receive_NACK();  // store data in data array and return NOT ACK
			  }
			  
		  }// for loop
	  }
  }
  TWI_STOP();											                // data has been received, Stop TWI
  
  while(1){
    for(unsigned int i=0;i<5;i++){                // show data on Serial Monitor
    Serial.println((char)received_data[i]);
    _delay_ms(300);
  }
    }
}

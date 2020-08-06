/*
* Learning AVR with Arduino
* Lesson: EEPROM
* Author: Morgan
* Date: 5/August/2020
* IDE: Arduino 1.8.13
*
* save data into EEPROM of Arduino UNO and read data.
*
*/


#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#define F_CPU 16000000UL

void EEPROM_write_byte(unsigned int uiAddress,unsigned char ucdata){
  while(EECR & (1<<EEPE));          // wait for completion of previous write
  EEARH = uiAddress >> 6;           // set up address register
  EEARL = 0xFF & uiAddress;
  EEDR = ucdata;                    // set up data  register
  EECR |= (1<<EEMPE);               // write logical one to EEMPE
  EECR |= (1<<EEPE);                // start EEPROM write by setting EEPE
  sei();                            // enable interrupt
}

unsigned char EEPROM_read_byte(unsigned int uiAddress){
  while(EECR & (1<<EEPE));          // wait for completion of previous write
  EEARH = uiAddress >> 6;           // set up address register
  EEARL = 0xFF & uiAddress;
  EECR |= (1<<EERE);                // start EEPROM read by setting EERE
  sei();                            // enable interrupt
  return EEDR;                      // return data from data register  
}

int main(){
  Serial.begin(9600);
  unsigned int addr = 0;              // address location
  unsigned int index = 0;             // data array index
  String data1 = "ABC\n";             // test data
  String get_data;                    // get data from EEPROM

  for(unsigned int i=0; i<data1.length(); ++i){
    EEPROM_write_byte(i, data1[i]);                  // write data1 to locate addr
  }

  while(EEPROM_read_byte(index)!= '\n'){
    get_data += (char)EEPROM_read_byte(index);       // read data from locate addr and assign to variable
    index++;
  }
  
  Serial.println((String)"Data Write into EEPROM is : " + data1);
  Serial.println((String)"Data Read from EEPROM is  : " + get_data);

  while(1){};
}

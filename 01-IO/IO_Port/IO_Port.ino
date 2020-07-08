/* Learning AVR with Arduino
 *  Lesson: I/O PORT
 *  Date: 25/June/2020
 *  Author: Morgan
 * 
 */

#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 16000000UL

int main(){
  DDRC |= (1<<PC0)|(1<<PC1);      // set PORTC0 and PORTC1 as output
  PORTC &= ~(1<<PC0)|~(1<<PC1);   // set PORTC0 and PORTC1 as LOW
  DDRB &= ~(1<<PB0);              // set PORCB0 as input
  Serial.begin(9600);             // for observing
  
  while(1){
    PORTC |= (1<<PC0);            // set PORTC0 as HIGH
    _delay_ms(300);               // delay 300 ms
    PORTC |= (1<<PC1);            // set PORTC1 as HIGH
    _delay_ms(300);               // delay 300 ms

    PORTC &= ~(1<<PC0);           // set PORTC0 as LOW
    _delay_ms(300);               // delay 300 ms
    PORTC &= ~(1<<PC1);           // set PORTC1 as LOW
    boolean pin_status = PINB & (1<<PINB0); // assign status of PORTCB0 to variable
    _delay_ms(300);               // delay 300 ms
    Serial.println(pin_status);   // print status of PORTB0 on serial monitor
    
  }
}




/* Arduino syntax
void setup(){
  pinMode(8,INPUT);        // set digital pin 8 (PORTB0) as output
  pinMode(A0,OUTPUT);      // set analog pin A0 (PORTC0) as output
  pinMode(A1,OUTPUT);      // set analog pin A1 (PORTC1) as output
  digitalWrite(A0,LOW);    // set pin A0 as LOW
  digitalWrite(A1,LOW);    // set pin A1 as LOW
  
  Serial.begin(9600);      // serial communicating
}

void loop(){
  digitalWrite(A0, HIGH);   // set pin A0 (PORTC0) as HIGH
  delay(300);               // delay 300 ms
  digitalWrite(A1, HIGH);   // set pin A1 (PORTC1) as HIGH
  delay(300);               // delay 300 ms

  digitalWrite(A0, LOW);    // set pin A0 (PORTC0) as LOW
  delay(300);               // delay 300 ms
  digitalWrite(A1, LOW);    // set pin A1 (PORTC1) as LOW
  delay(300);               // delay 300 ms

  boolean pin_status = digitalRead(8);  // assign status of PORTB0 to variable
  Serial.println(pin_status);           // show status of PORTB0 on serial monitor
}
*/

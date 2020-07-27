/*
* Learning AVR with Arduino
* Lesson: Analog Digital Convert, ADC
* Author: Morgan
* Date: 25/July/2020
* IDE: Arduino 1.8.13
* 
* ADC = Vin * 1024 / Vref
* Vin = ADC * Vref / 1024
* 
* Sampling rate for ADC is about 50K ~ 200K Hz.
* 
*/

#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
#define F_CPU 16000000UL

volatile unsigned int adc_value = 0;
ISR(ADC_vect){
  // read result of convert
  adc_value = ADCL;
  adc_value += (ADCH << 8);
}

void ADC_setup(){
  ADMUX = 0;                             // clear ADMUX register
  ADMUX |= (1<<REFS0);                   // AVCC voltage reference,it connects to +5V in Arduino UNO

  // ADEN:enable ADC. ADIE:enable interrupt. ADPS[2:0]=111: prescaler 128
  ADCSRA |= (1<<ADEN)|(1<<ADIE)|(ADPS2)|(1<<ADPS1)|(1<<ADPS0);
  ADCSRA |= (1<<ADSC);                   // start to convert
  sei();                                 // enable global interrupt
}

void pin_setup(){
  DDRC &= ~(1<<PC0);                     // set PORTC0 as input
}


int main(){
  pin_setup();
  ADC_setup();
  float adc_voltage = 0;
  Serial.begin(9600);
  
  while(1){
    if(!(ADCSRA & (1<<ADSC))){
      // if convert finished, ADSC will be cleared.
      adc_voltage = (float)adc_value*5/1024;
      Serial.print("ADC Result : ");
      Serial.println(adc_value);
      Serial.print("Convert to voltage is : ");
      Serial.print(adc_voltage);
      Serial.println("  [V]");
      _delay_ms(3000);                        // delay 3000 ms
      ADCSRA |= (1<<ADSC);                    // start to convert again
     }
    }
}

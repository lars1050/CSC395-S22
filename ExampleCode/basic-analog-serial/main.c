
#define F_CPU 16000000
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>

#include "analog.h"
#include "serial.h"

void initialize_heartbeat() {
  	DDRD |= (1<<DDD5);
}

void heartbeat_with_delay() {
  PORTD ^= (1<<DDD5);
  _delay_ms(250);
}

int main() {

  // initialize communication
  char buffer[50];
  setupUART();

  // initialize power supply
  // I use this generally to plug into breadboard to power everything
  // AStar pin 11
  DDRB |= ( 1 << DDB7 );
  PORTB |= ( 1 << PORTB7 );

  // Status Report of System Bootup
  initialize_heartbeat();
  heartbeat_with_delay();
  heartbeat_with_delay();
  heartbeat_with_delay();
  heartbeat_with_delay();

  // initialize ADC for potentiometer
  uint16_t adc_results;
  adc_init();

  _delay_ms(5000);  // give user 5 seconds to establish communication

  printf("All Systems Go\n");

  sei();

  while(1)
  {
    adc_results = adc_read(10);
    sprintf(buffer,"%d\r\n",adc_results);
    sendString(buffer);
    heartbeat_with_delay();

    adc_results = adc_read(9);
    sprintf(buffer,"%d\r\n",adc_results);
    sendString(buffer);
    heartbeat_with_delay();
  }
}

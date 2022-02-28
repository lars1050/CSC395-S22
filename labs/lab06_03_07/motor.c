/*
* basic_motor.c
*
* Created: 02/2017
* Author : Chris Owens and Amy Larson
*/
#include "motor.h"

// comment out line if not debugging
#define DEBUG_PCINT

// For setting frequency of timer. Freq calculation based on TOP+1
#define TOP_4kHz 3999

/* PCINT for Encoder was ported from
*  PololuWheelEncoders.cpp
*/

#ifdef DEBUG_PCINT
volatile uint32_t interrupt_counter = 0;
#endif

volatile int8_t global_m2a;
volatile int8_t global_m2b;

volatile int16_t global_counts_m2 = 0;
volatile int8_t global_error_m2 = 0;

volatile int16_t global_last_m2a_val;
volatile int16_t global_last_m2b_val;

void setupMotor2(void) {

  // Turn data direction to input to turn motor off
  DDRE &= ~(1 << DDE2);
  
  // Data Direction to output for Motor pin
  DDRB |= (1 << PINB6);
  
  // Configure the motor direction pin to output
  m2_dir_control |= (1<< m2_dir_pin );

  // start off going "forward" with the intent of increased encoder counts
  motorForward();

  // Motor 2 is connected to Timer0 Channel B
  //Clear the registers
  TCCR1A = 0;
  TCCR1B = 0;
  ICR1 = 0;

  // Clearing on match for non-inverted signal (i.e. larger match value produces larger duty cycle)
  TCCR1A |= (1 << COM1B1);

  /* want mode 14 (1110), ICR1 is top, OCR1B is match */
  TCCR1A |= (1 << WGM11);
  TCCR1B |= (1 << WGM12) | (1 << WGM13);

  // Using 1 as prescaler (001)
  /* 4kHz counter frequency, period of 1/4000 = .25ms period */
  TCCR1B |= (1 << CS10);
  ICR1 = TOP_4kHz;

  // Turn data direction to output to turn motor on
  OCR1B = 0;  // speed 0
  DDRE |= (1 << DDE2);  // motor on
}

void setupEncoder(void) {

  // Set the encoders as input
  chA_control &= ~(1 << chA_pin );
  chB_control &= ~(1 << chB_pin );

  // Enable the interrupts for the 2 encoder channels
  PCMSK0 |= (1 << chA_INT );
  PCMSK0 |= (1 << chB_INT );

  // enable PCINT interrupts
  PCICR |= (1 << PCIE0 );

  // Powering the encoder through general I/O. This sets signal high to provide power to device.
  enc_power_control |= (1 << enc_power_pin );
  enc_power_output  |= (1 << enc_power_pin );
}

ISR(PCINT0_vect){
  #ifdef DEBUG
  ++interrupt_counter;
  #endif

  // Make a copy of the current reading from the encoders
  uint8_t tmpB = PINB;

  // Get value of each channel, making it either a 0 or 1 valued integer
  uint8_t m2a_val = (tmpB & (1 << chA_pin )) >> chA_pin;
  uint8_t m2b_val = (tmpB & (1 << chB_pin )) >> chB_pin;

  // Adding or subtracting counts is determined by how these change between interrupts
  int8_t plus_m2 = m2a_val ^ global_last_m2b_val;
  int8_t minus_m2 = m2b_val ^ global_last_m2a_val;

  // Add or subtract encoder count as appropriate
  if(plus_m2) { global_counts_m2 += 1; }
  if(minus_m2) { global_counts_m2 -= 1; }

  // If both values changed, something went wrong - probably missed a reading
  if(m2a_val != global_last_m2a_val && m2b_val != global_last_m2b_val) {
    global_error_m2 = 1;
    PORTD ^= (1 << PORTD5);
  }

  // Save for next interrupt
  global_last_m2a_val = m2a_val;
  global_last_m2b_val = m2b_val;

  // If trying to debug, flash an led so you know the PCINT ISR fired
  #ifdef DEBUG_PCINT
  if (0 == interrupt_counter%20 ) {
    PORTD ^= (1 << PORTD5 );
  }
  #endif
}

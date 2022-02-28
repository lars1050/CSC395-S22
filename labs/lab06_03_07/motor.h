/*
 * basic_motor.h
 *
 * Created: 02/2017
 * Author : Chris Owens and Amy Larson
 */

#include "common.h"

#include <avr/interrupt.h>

void setupMotor2(void);
void setupEncoder(void);

// Motor 2 PWM signal generated on B6
#define m2_port PORTB
#define m2_pin PORTB6
#define m2_control DDRB

// The direction of motor 2 is controlled on Port E pin 2
#define m2_dir_control DDRE
#define m2_dir_pin DDE2
#define m2_dir_output PORTE

// Set the direction (arbitrarily) to 0.
// Whether this generates positive or negative encoder counts depends on how motor is plugged in.
#define motorForward()  (m2_dir_output &= ~(1 << m2_dir_pin ))
#define motorBackward() (m2_dir_output |=  (1 << m2_dir_pin ))

// Encoder channels (yellow and white wires) must be plugged in to PB4 and PB5 (Astar pins 8 and 9)
// You can plug in elsewhere, but then this code needs changing.
#define chA_control DDRB
#define chA_pin DDB4
#define chB_control DDRB
#define chB_pin DDB5
// Interrupt #s for encoder are based on where they are plugged in. See AStar pinout PB4 = PCINT4
#define chA_INT PCINT4
#define chB_INT PCINT5

// Powering the encoder through general I/O. Plug power (blue) in to Port D Pin 1 (AStar pin 2!!)
// You can put these anywhere you like, but moving blue means changing this code.
// Plug green into any ground in general I/O.
#define enc_power_control DDRD
#define enc_power_pin DDD1
#define enc_power_output PORTD

/* PCINT for Encoder was ported from
*  PololuWheelEncoders.cpp
*/
extern volatile int16_t global_counts_m2;
extern volatile int8_t global_error_m2;

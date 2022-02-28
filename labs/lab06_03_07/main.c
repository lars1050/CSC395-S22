#include "common.h"

#include <avr/interrupt.h>
#include <util/delay.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "motor.h"
#include "leds.h"


/****************************************************************************
   ALL INITIALIZATION
****************************************************************************/
void initialize_system(void)
{
  // initalize green and yellow only.
	// initialization defines the IO_structs and sets DDR
	initialize_led(GREEN);
	initialize_led(YELLOW);

	// The "sanity check".
	// When you see this pattern of lights you know the board has reset
	light_show();

  setupMotor2();
  setupEncoder();
  motorForward();
}

void blink_yellow() {
  TOGGLE_BIT(*(&_yellow)->port, _yellow.pin);
  _delay_ms(300);
  TOGGLE_BIT(*(&_yellow)->port, _yellow.pin);
}

void move_motor() {
  // going faster
  OCR1B = 0;
  _delay_ms(500);
  OCR1B = 250;
  _delay_ms(500);
  OCR1B = 500;
  _delay_ms(500);
  OCR1B = 750;
  _delay_ms(500);
  OCR1B = 1000;
  blink_yellow();

  // going slower
  OCR1B = 750;
  _delay_ms(500);
  OCR1B = 500;
  _delay_ms(500);
  OCR1B = 250;
  _delay_ms(500);
  OCR1B = 0;
  blink_yellow();
}

/****************************************************************************
   MAIN
****************************************************************************/

int main(void) {
  // This prevents the need to reset after flashing
  USBCON = 0;

	initialize_system();

  sei();
  while(1) {

    move_motor();
    motorBackward();
    move_motor();
    motorForward();

  } /* end while(1) loop */
} /* end main() */

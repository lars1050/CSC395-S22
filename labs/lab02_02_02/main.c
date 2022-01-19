#include "common.h"

#include <avr/io.h>
#include <util/delay.h>

#include "leds.h"
#include "buttons.h"

#define INVERTED 1

/****************************************************************************
   ALL INITIALIZATION
****************************************************************************/
void initialize_system(void) {
	// initalize green and yellow only.
	// initialization defines the IO_structs and sets DDR
	initialize_led(GREEN);
	initialize_led(YELLOW);

	// The "sanity check".
	// When you see this pattern of lights you know the board has reset
	light_show();

	// initalize only buttonA and buttonC because they are connected to PCINT
	// NOTE: button C and the RED led are on the same line.
  initialize_button(BUTTONA);
	initialize_button(BUTTONC);
}

/****************************************************************************
   MAIN
****************************************************************************/

int main(void) {
  // This prevents the need to reset after flashing
  USBCON = 0;

	initialize_system();

  //*************************************************************//
  //*******         THE CYCLIC CONTROL LOOP            **********//
  //*************************************************************//

  // FILL THIS IN TO MEET FUNCTIONAL REQUIREMENTS:

	// Always toggle yellow LED every 1000ms
	// Toggle greed LED every 500ms, but only if is in an on state
	// Press and release button A switches green to on state.
	// Press and release button C switches green to off state.

	// Assume both buttons start in a not pressed state.

	uint8_t green_on = 1;

  while(1) {

		_delay_ms(1000);
		led_toggle(&_yellow);
		if (green_on) {
			led_toggle(&_green);
		} else {
			led_off(&_green,INVERTED);
		}

	} // end while(1)

} /* end main() */

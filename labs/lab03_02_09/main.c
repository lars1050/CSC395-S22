#include "common.h"

// Comment this to remove "heartbeat" and led flash in PCINT
//#define DEBUG 1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#include "leds.h"
#include "buttons.h"

void blink_yellow();

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

	// initalize only buttonA and buttonC because they are connected to PCINT
	// NOTE: button C and the RED led are on the same line.
  initialize_button(&_buttonA);
	initialize_button(&_buttonC);

  //DDRB &= ~(1<<BUTTONC);
  //PORTB |= (1<<BUTTONC);

  //CLEAR_BIT(*(&_buttonC)->ddr, _buttonC.pin);
  // enable pull-up resistor
  //SET_BIT(*(&_buttonC)->port, _buttonC.pin);

  setup_button_action(&_interruptC, 0, blink_yellow);

  //enable_pcint(&_interruptA);
  enable_pcint(&_interruptC);

}

void blink_yellow() {
  TOGGLE_BIT(*(&_yellow)->port, _yellow.pin);
  _delay_ms(300);
  TOGGLE_BIT(*(&_yellow)->port, _yellow.pin);
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

  // FILL THIS IN TO BLINK LEDS at the requested frequency given the
  // current state of the system (i.e. if a button was pressed or released)
  // Even if you know how to set up a timer interrupt, do not use it
  // for this assignment. You may use _delay_ms()

  // without keyword volatile, the compiler optimizes away count
  volatile uint32_t count = 0;

  sei();
  while(1) {

    #ifdef DEBUG
    // might be useful to see a heartbeat if in debug mode
    ++count;
    if (count == 50000) {
      TOGGLE_BIT(*(&_yellow)->port, _yellow.pin);
      count = 0;
    }
    #endif

  } /* end while(1) loop */
} /* end main() */

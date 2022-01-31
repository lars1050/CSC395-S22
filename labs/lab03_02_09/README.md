##### CSC395 RTES Spring 2022

#### Lab 3 : Blinking LEDs with Button Interrupts
> CODE DUE: Wednesday, February 9 before class <br>
> STATE MACHINE sketch DUE: Wednesday, February 2 end of class<br>
> Submit via Moodle (zipped).

<hr>

**__FUNCTIONAL REQUIREMENTS__**

1. Do not use any timer interrupts. Use only the PCINT ISR.

1. Start the system with both LEDs off.

1. Onboard buttons modify LED functionality (state) in the following way:
	- Button A: modifies only the Yellow LED state
	- Button C: modifies only the Green LED state
	- 1st **__release__**: LED ON (solidly, no blinking)
	- 2nd **__release__**: LED Blink at frequency provided below
	- 3rd **__release__**: LED OFF

1. When an LED is in a blinking state:
	- GREEN LED
		- blink at 2 Hz (ON at 250 ms, OFF at 500 ms, ON at 750 ms, OFF at 1 sec, ...)
	- YELLOW LED
		- blink at .4 Hz (ON at 1250 ms, OFF at 2500 ms, … ).

> The green and yellow LED are independent. Either can be in any state regardless of the state of the other.

## Coding

Start by copying your completed lab 2 to a new folder to start on lab 3. Look through buttons.h and buttons.c below and add the code to your lab 3 project. Complete the code for buttons.c and make sure it compiles before moving on.

Test your new code by creating a function that blinks or toggles an LED. Set up a button to use this function as a callback for when the button is pressed. Then change it to be the callback for releasing the button to ensure everything is working properly.

Now you are ready to tackle main(). Use the concept of a state machine to sketch out how to manage the LED state and behavior. Keep in mind that the two leds are independent, so you can design them as separate state machines. All control of the LEDs should be managed from inside main (i.e. do not turn an LED on or off as part of the ISR, instead set variables to control the state of the system). Set up your "state machine(s)" in the while loop in main.

Finally, write the callback functions and set up the button actions as appropriate. Again, do not control your LEDs as part of the ISR callbacks. Think of the LED control as a task, therefore the control loop should be in control of the execution of that task.

<hr>

#### buttons.h

Here is a new version of buttons.h

```C
#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "common.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BUTTONA 3
#define BUTTONC 0

#define PCINT_BUTTONA PCINT3
#define PCINT_BUTTONC PCINT0

typedef struct {
  uint8_t pcint;
  uint8_t mask;
  uint8_t enabled;
  uint8_t prev_state;
  void (*release_fn)(void);
  void (*press_fn)(void);
} INTERRUPT_struct;

extern IO_struct _buttonA;
extern IO_struct _buttonC;

extern INTERRUPT_struct _interruptA;
extern INTERRUPT_struct _interruptC;

/* Enable the PCINT for any of the buttons on the board.
 * This will set up for a PCINT ISR. Don't forget to call sei() in main.
 * param[in] INTERRUPT_struct *state : for A or C as defined above
*/
void enable_pcint(INTERRUPT_struct *state);

/* Set up callback function for any of the buttons on the board.
 * param[in] INTERRUPT_struct *state: for A or C as defined above
 * param[in] release: 1 (true) = callback on release, else callback on press
 * param[in] callback : function to be called when event happens
 */
void setup_button_action(INTERRUPT_struct *state, int release, void(*callback)(void));

/* Used as a callback placeholder when button is not enabled */
void empty_function();

// Setup is for A and C only because button B does not have corresponding
// PCINT for easy interrupt programming.
//
// WARNING: ButtonC and RED LED share the pin. Do not use both.
void initialize_button(IO_struct * button);

#endif
```

#### buttons.c

Here is some of buttons.c to get you started:

```C
#include "buttons.h"
#include "leds.h"

// comment this line out when you are done debugging
#define DEBUG

uint8_t button_mask = 0;

IO_struct _buttonA = { &DDRB, &PORTB, BUTTONA, &PINB };
IO_struct _buttonC = { &DDRB, &PORTB, BUTTONC, &PINB };

INTERRUPT_struct _interruptA =
{ PCINT_BUTTONA, (1<<BUTTONA), 0, (1<<BUTTONA), empty_function, empty_function};

INTERRUPT_struct _interruptC =
{ PCINT_BUTTONC, (1<<BUTTONC), 0, (1<<BUTTONC), empty_function, empty_function};

void empty_function() {}

// configure the system to use the button
void initialize_button(IO_struct * button) {
  CLEAR_BIT(*button->ddr, button->pin);
  // enable pull-up resistor
  SET_BIT(*button->port, button->pin);
}

void enable_pcint(INTERRUPT_struct *state) {

  // PCICR: Pin Change Interrupt Control Register
  // PCIE0: Pin Change Interrupt Enable Bit:
  //    Any change on any enabled PCINT7..0 can fire ISR.
  PCICR |= (1 << PCIE0);

  // PCMSK0: Pin Change Mask for Interrupt0, which is for all pins 0 through 7
  // Enable interrupts on Button A (PCINT3) and Button C (PCINT0)
  PCMSK0 |= (1 << state->pcint);

  state->enabled = 1;
  button_mask |= state->mask;
  state->prev_state = PINB & state->mask;
}

int setup_button_action(
  INTERRUPT_struct *state, int release, void(*callback)(void)) {
    if (release) {
      state->release_fn = callback;
    } else {
      state->press_fn = callback;
    }
}

ISR(PCINT0_vect) {

  // read port B, mask for enabled buttons
  uint8_t pinb_now = (PINB & button_mask);

  // check that pin change persists. ignore if not.
  _delay_ms(10);
  if (pinb_now ^ (PINB & button_mask)) {
    return;
  }

  #ifdef DEBUG
  // If debugging, it will be good to flash an led to know that you made
  // it into the ISR. Remember that red and buttonC share the port, so don't
  // use the red led. If you soldered your headers, you can also set up
  // an LED in general IO to give yourself more options.
  TOGGLE_BIT(*(&_green)->port, _green.pin);
  _delay_ms(150);
  TOGGLE_BIT(*(&_green)->port, _green.pin);
  #endif

  // ************************************************************/
  // ***********    COMPLETE THIS PART BELOW ********************/
  // ************************************************************/
  // record which button(s) changed state from last interrupt
  // remember that pinb_now holds state for only button A and C
  uint8_t state;
  if (_interruptA.enabled) {


    // if there was a state change

      // if it was pressed, call the press_fn()
      // else, call the release_fn()
      // save state as prev_state
    }
    // repeat for button C
}
```

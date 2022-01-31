#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "common.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BUTTONA 3
#define BUTTONC 0

// NEW BETWEEN HERE >>>>>>>>>>>>>>>>>>>>>>>>

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

/* Enable the PCINT for any of the buttons on the board.
 * This will set up for a PCINT ISR. Don't forget to call sei() in main.
 * parameter [out] : -1 for error in setting up. 1 otherwise.
 * parameter [in] IO_struct button : for A or C as defined above
*/
void enable_pcint(INTERRUPT_struct *state);

/* Set up callback function for any of the buttons on the board.
 * parameter [in] IO_struct button : for A or C as defined above
 * parameter [in] release : 1 (true) = call function on release
 * parameter [in] callback : function to be called when event happens
 * parameter [out] : -1 for error in setting up. 1 otherwise.
 */
int setup_button_action(INTERRUPT_struct *state, int release, void(*callback)(void));

void empty_function();

/* These function pointers can be used to generalize the PCINT code.
 * Rather than writing a new PCINT for every program to achieve different
 * behavior, you can always use the same PCINT code for buttons.
 * For example, I might write 2 functions that 1) blinks the red led every
 * time button A is released and 2) blinks the green led every time it is
 * pressed.
 * If I called these functions "void blink_red()" and "void blink_green()"
 * I would set fn_release_A = blink_red and fn_press_A = blink_green.
 * Note that the function signature (i.e. void fn(void)) must match.
 * If you don't want the buttons to have any functionality, then you can
 * set these function pointers to the EmptyFunction.
 */
/*
void (*fn_release_A)(void);
void (*fn_press_A)(void);
void (*fn_release_C)(void);
void (*fn_press_C)(void);

volatile uint8_t pinb_previous;
*/
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<   AND HERE

// Setup is for A and C only because button B does not have corresponding
// PCINT for easy interrupt programming.
//
// WARNING: ButtonC and RED LED share the pin. Do not use both.

extern IO_struct _buttonA;
extern IO_struct _buttonC;

extern INTERRUPT_struct _interruptA;
extern INTERRUPT_struct _interruptC;

void initialize_button(IO_struct * button);

int is_button_pressed(IO_struct * button);

#endif

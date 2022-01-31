#ifndef LEDS_H_
#define LEDS_H_

#include "common.h"

#include <util/delay.h>
#include <inttypes.h>

// These are being used like enumerator types, except not in order but instead
// using pin numbers as enumeration.
#define GREEN 5
#define YELLOW 7
#define RED 0

// Global variables. They MUST be defined using initialize_led(<COLOR>).
IO_struct _yellow;
IO_struct _red;
IO_struct _green;

/* initialize all the on-board leds by calling the macros defined above
 * for initializing the individual on-board leds
 */
void initialize_led(int color);

/* turn on specified on board LED. indicate if inverted (0 turns led on)
*/
void led_on(IO_struct * color, int inverted);

/* turn off specified on board LED. indicate if inverted (1 turns led off)
*/
void led_off(IO_struct * color, int inverted);

/* toggle the led between on and off
*/
void led_toggle(IO_struct * color);

/* Flash the designated on-board led for 250ms on, then 250ms off.
 * Assumes led is initialized */
void flash_led(IO_struct * color, int inverted);

/* Flash all the leds for a sanity check light show */
void light_show();

#endif

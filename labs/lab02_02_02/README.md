##### CSC395 Real-Time Embedded Systems Spring 2022

#### Lab 2 : Blinking LEDs
> DUE: Wednesday, February 2 before class <br>
> Submit via Moodle. Zip all files.

<hr>

Objectives:
- Understand C compilation including preprocessing directives like #define.
- Gain experience developing support code for common functionality.
- Appreciate the difficulty of managing multiple tasks under time constraints.

This lab is about establishing good coding practices and getting support code established for all future programming. You will establish header and source files for LED and button functionality. In addition, you will create a common.h file for common definitions and macros. If you do not know C, this code will be hard to follow (it might even be hard to follow if you do know C). We will go through it in class.

Once the support code is established, you will write code to blink LEDs at a specific frequency and toggle the led state on and off using buttons.

### 1. `common.h`

Create a file `common.h`. Start with the header guards. When you compile code, it is often the case that multiple files `#include` the same header file, and sometimes there are circular references, for example headerA.h includes headerB.h and headerB.h includes headerA.h. If this happens, the compiler infinitely goes between them. A header guard is a way to only include a header one time. It is common programming practice to use `FILENAME_H_` for the guard, where FILENAME refers to the name of the file in which it is contained.

##### Explanation of Code Below

`#ifdef` or `#ifndef` (if defined or not defined, respectively), is a directive to the compiler to compile that block of code only if this is true. In this situation, it is used for more efficient compilation, but there are other uses for it as we will see later.

The first time through this common.h file, COMMON_H_ is not defined, so it continues. Immediately, COMMON_H_ is defined, so the second time through it will ignore everything up to the `#endif`.

```
#ifndef COMMON_H_
#define COMMON_H_

// Your code will go here in between the guarded block

#endif
```

Right after the #define, define F_CPU, which stands for frequency of CPU. This is useful when reusing code across different processors with different clock speed.

```
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
```

Now add these lines of code, which are essential for almost everything we do on this board.

```
#include <avr/io.h>
#include <inttypes.h>
```

Add definitions for each pin. The avr files have definitions for the pins, however they distinguish between the ports (e.g. `#define PINA3 3`), which seems cumbersome to me. One could apply the same argument to the use of any names for the pins for these general ports, but I think `(1 << PIN3)` is easier to read and more consistent with other registers whose pins need to be named (you will see this later).

```
#define PIN0 0
#define PIN1 1
#define PIN2 2
#define PIN3 3
#define PIN4 4
#define PIN5 5
#define PIN6 6
#define PIN7 7
```

Add these very useful _macros_ to easily set, clear, and toggle a single bit within a register. (_Toggle_ will toggle the value of the bit between 0 and 1.) These are essentially inline function calls that are efficient while making code more readable and less prone to typos. `SET_BIT(DDRB,2)` for example will set bit 2 in the DDRB register to 1.

```  
#define SET_BIT(reg, pin) (reg |= (1<<pin))
#define CLEAR_BIT(reg, pin) (reg &= ~(1<<pin))
#define TOGGLE_BIT(reg, pin) (reg ^= (1<<pin))
```

Add 2 more useful macros so that you never have to remember if 1 in a DDR register means it is configured as output or input!

```
#define CONFIG_OUTPUT(port, pin) (SET_BIT(port,pin))
#define CONFIG_INPUT(port,pin) (CLEAR_BIT(port,pin))
```

The last piece to include to common.h is a struct definition. Think of structs  as very simple classes that just hold data without any supporting methods. Note that `uint8_t` is an unsigned integer of length 8 bits. Other examples of this are `int8_t`, `int16_t`, and `uint16_t`. It is good to be in control of the size of your variables so that you can have consistency across devices.

Add this to common.h:
```
typedef struct {
  volatile uint8_t * ddr;
  volatile uint8_t * port;
  uint8_t pin;
  volatile uint8_t * portin;
} IO_struct;
```
##### Structs and Pointers Quick Reference

The structure definition above defines a user-defined Type called `IO_struct` that contains 5 variables. It can be used to declare a variable just like any other type such as `int` or `float`. For example,
```
IO_struct buttonA = (IO_struct) { &DDRB, &PORTB, PINB3, &PINB };
```

Notice the use of `*` and `&`. These are pointer operators! Most languages do not allow you to get down to the memory and reference it directly, but C does. The tradeoff is that you have to be very careful when using pointers! Remember this:
- `&` the Ampersand is used to get the Address of a variable.
- `*` is a type specifier in a variable declaration
- `*` dereferences a pointer (i.e. gets the data stored at the memory location that is being pointed to) when used in a statement.

For example,
```
int myInt = 0;
int * pInt = &myInt;
printf("%d %d %d %d", myInt, *pInt, pInt, &myInt);
```
declares a variable of type `int`, then declares a variable of type `int *` (a pointer to an int), then sets the value of that variable to the memory address of myInt. The output of the print statement would be "0 0" followed by the memory address of myInt 2 times.

In C, it is your responsibility to manage your pointers correctly. For example,
```
pInt = 0;
printf("%d",*pInt);
```
is syntactically correct -- it will compile. But now pInt points to address 0 in memory, which you definitely do not want. Now when you try to print the data at memory address 0, the system will crash with a SEGFAULT error. If you get a SEGFAULT error, 50% of the time it is probably due to not managing pointers correctly (like above) and the other 50% is indexing outside the range of an array or struct. Unfortunately, the SEGFAULT error is generated by an OS, which we do not have. Instead, on an embedded system, the board resets and you will see your "light show" to indicate something went wrong.

You may add other definitions to this file as you develop code throughout the semester.

<hr>

### 2. leds.h and leds.c

Now, let's build some support code for the on-board LED functionality. We want to create easy to read and easy to use variables and functions. Create the file `leds.h`. The file `leds.c` has been provided.

Add the header guard to `leds.h` following the form in _common.h_, but using `#ifndef LEDS_H_` instead of `#ifndef COMMON_H_`.

Include these files:
```
#include "common.h"
#include <util/delay.h>
#include <inttypes.h>"
```

Add these definitions and declarations:

```
// These function as enumerated types (using pin numbers as values)
#define GREEN 5
#define YELLOW 7
#define RED 0

// Global variables. They MUST be defined using initialize_led(<COLOR>).
IO_struct _yellow;
IO_struct _red;
IO_struct _green;
```

Declare the following functions, which are defined in `leds.c`.

```
/* initialize the data struct for the specific on-board led.
 */
void initialize_led(int color);

/* Flash the designated on-board led for 250ms on, then 250ms off.
 * Assumes led is initialized */
void flash_led(IO_struct * color, int inverted);

/* Flash all the initialized leds for a sanity check light show */
void light_show();
```

Declare these function in leds.h and define them in leds.c.

```
/* turn on specified on board LED. indicate if inverted (0 turns led on)
*/
void led_on(IO_struct * color, int inverted);

/* turn off specified on board LED. indicate if inverted (1 turns led off)
*/
void led_off(IO_struct * color, int inverted);

/* toggle the led between on and off
*/
void led_toggle(IO_struct * color);
```

<hr>

### 3. buttons.h and buttons.c

Now, let's build some support code for the on-board button functionality. We want to create easy to read and easy to use variables and functions. Create the file `buttons.h`. The file `buttons.c` has been provided.

Add the header guard to `buttons.h` following the form in _common.h_, but using `#ifndef BUTTONS_H_` instead of `#ifndef COMMON_H_`.

Include these files:
```
#include "common.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
```

> We won't be using interrupts this week, but next week ...

Add these button pin definitions and button struct declarations:

```
#define BUTTONA 3
#define BUTTONC 0

// These must be defined using initialize_button()
IO_struct _buttonA;
IO_struct _buttonC;
```


Declare the following functions, which are defined in `buttons.c`.

```
/* Initialize the specified on-board button.
* The corresponding pin is set as input.
* The pullup resistor is enabled.
*/
void initialize_button(int button);

/* Check if the user-specified button is pressed. Return 1 if so, 0 if not.
* The bit value in the PINB register is 0 when pressed, 1 when not pressed.
*/
int is_button_pressed(IO_struct * button);
```

<hr>

Check that everything compiles using the provided makefile. You should see the green and yellow LED flash twice as part of the light show. Then the yellow and green led should blink about 1 time per second.

<hr>

### 4. Blinking LEDs

Now you can turn to programming the required behavior.

- Constantly toggle the yellow LED at a rate of 1000ms (i.e. on for 1000ms, off for 1000ms, on for 1000ms, ...). In other words, this "task" is running at a frequency of 1Hz (1 time a second).

- If the green LED is set to "on", it should toggle at a rate of 500 ms (i.e. on for 500ms, off for 500ms, on for 500ms, ...). In other words, this "task" is running at a frequency of 2Hz (2 times a second).

- A **press and release** of button A turns the green LED on (in a blinking state).

- A **press and release** of button C turns the greed LED off.

>For example, when it first starts, the green LED is off. The user presses button A and after button A is released, the green LED starts blinking at the specified frequency. Then, the user presses and releases button C, so the green LED turns off. If the LED is on when button A is pressed and released, there is no change to the system. If the LED is off when button C is pressed and released, there is no change to the system.

- The behavior is undefined it both buttons are pressed. (It is okay here to make the bad assumption that the user is cooperative and would never push both buttons at the same time or start the board up with the button already pressed, etc.)

Note that with your handy dandy support code, you can toggle the yellow LED with this line of code:
```
led_on(&_yellow,1); // This is inverted, hence the "1"
```

And check for button A being pressed using this line of code:
```
uint8_t state = is_button_pressed(&_buttonA);
```

You can use _delay_ms(#), but keep in mind this is what we call a "busy-wait." The system stops executing code during the delay. Think about how you will get your LEDs blinking at different frequencies all while checking for button presses and releases. And you cannot use interrupts, even if you know how.

**It will be hard to blink at exactly the required frequency. Get as close as you can. This lab is to get you thinking about how well you can control timing under these circumstances. ANSWER: not very well!**

ALL DONE! Congrats.

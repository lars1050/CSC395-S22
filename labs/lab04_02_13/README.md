##### CSC395 RTES Spring 2022

#### Lab 4 : Timers and Interrupts
> CODE due Wednesday, Feb 16 before class <br>
> WRITTEN HOMEWORK due Friday, Feb 18 end-of-day<br>

<hr>

##### WRITTEN HOMEWORK (due Friday, Feb 18 end-of-day)

1. What is the maximum period (i.e. minimum frequency) achievable with an 8-bit timer? Keep in mind that you need to select a prescaler before doing the calculations. Show your work.

2. What is the maximum period (i.e. minimum frequency) achievable with a 16-bit timer?

3. Imagine that a peripheral signals a microprocessor that it is ready to communicate by setting a pin high. That pin is monitored by a PCINT and it will trigger an ISR any time it changes. If the peripheral wants to communicate very, very, very often, what will be the impact on the system overall?

4. You have "scheduled" tasks by a) putting everything in the cyclic executive, b) by using PCINT ISRs, c) by using TIMER ISRs, and d) through self-scheduling in the cyclic executive. Think of an embedded system with peripherals (i.e. sensors, motors or other devices) and for 2 of the above methods, identify a peripheral in which it would be good to use that method and one in which it would be bad to use that method. Describe the system generally. For each scheduling method, describe the peripheral and briefly justify why you think that peripheral would be good/bad to schedule in that way.

**__HARDWARE READINESS__**

1. Solder the two 3x7 headers into the middle general purpose through-hole mounts.
2. Connect a red led to General Purpose I/O (GPIO) PortD pin 2. (This is pin 0 of the Pololu board on the header -- the one at the edge of board.)
3. Connect a yellow led to GPIO PortD pin 1. (This is pin 2 of the Pololu board.)
4. Connect a green led to GPIO PortD pin 4. (This is pin 4 of the Pololu board.)

> To connect an LED to GPIO, you have to make a circuit between ground and the port pin, and it must have a **resistor** in that circuit. Use your breadboards and jumper cables (OR make cables with the provided parts) to make the circuit. LEDs have to be plugged in the right way (the shorter pin connects to ground).

There is a very nice tutorial on LEDs on adafruit. Here is a link to one of the pages that shows you how to connect LEDs to your board: https://learn.adafruit.com/all-about-leds/what-are-leds-used-for.

### SYSTEM REQUIREMENTS

Implement the following behaviors:

1. Blink the GPIO RED LED (PD2) at 1Hz (on at 500ms, off at 1000ms, ...)
1. Blink the GPIO YELLOW LED (PD1) at 2Hz (on at 250ms, off at 500ms, ...)
2. Blink the GPIO GREEN LED PD4) at 2.5Hz (on at 200ms, off at 400ms) 5 times in response to the release of Button A.

> The on-board LEDs are not being used as a task so that you can use them for debugging. Blinking lights are a lot more helpful than you would think.

This behavior must be achieved using the following architecture:

1. To blink RED, **__background self-schedule__** in the main loop, by observing system time (as maintained by an ISR) and toggling when appropriate. E.g.
```
if (ms_timer >= release_time) { // toggle }
```
You are free to use some other conditional statement that will achieve the same logic.

2. To blink YELLOW, __**foreground time-triggered scheduling**__ by creating a timer dedicated to this task (i.e. separate from the one used as a system clock) in which a timer interrupt fires every 250ms, and the led is toggled inside the corresponding ISR.

3. To blink Green, __**foreground event-triggered scheduling**__ that is triggered by the button A release. The blinking of the led occurs **in the ISR**.

### LED Configuration and Control

The LEDs that you plugged into your breadboard operate identically to the ones built in:

- configure the appropriate pins in the DDRD register
- set the appropriate pin on PORTD (set it high to 1) to turn it on
- clear the pin on PORTD (set it low to 0) to turn it off

You can choose to add code to your _leds.h_ and _leds.c_ files or make a new set of files called _gpio.h_ and _gpio.c_ for general IO control.

> It is very important that you have a "light show" sanity check with these GPIO LEDs after they are initialized and before you start up your interrupts with `sei()`. LEDS can be plugged in backwards or in the wrong port or a wire could be loose or the LED burnt out or your code could be wrong -- many reasons why the led might not light up! You want to make sure it is functional so that when it doesn't toggle when the system is up and running, you know it is your software, not your hardware. **REALLY, YOU MUST DO THIS!!!**

Once your interrupts are enabled with `sei()`, at any point after that, if you see this light show again, your board reset. Check if you enabled an interrupt but you do not have a corresponding ISR for when it fires. This will send the program counter to the ether and crash the board.

### TIMER 0 and the Red LED

There is no requirement for this lab that your timer interrupt code be well organized -- hack away -- and I think I would recommend this. It is hard to organize something you don't yet understand. I will ask you to organize it in the next lab (or maybe the one after that). You eventually want reusable code to easily configure timers.

You will be setting up timer0 (8-bit timer) to manage the "clock" which will update the global variable `ms_ticks`. As the name implies, this variable reflects the number of milliseconds that have passed since the system started. Therefore, you want the interrupt to fire every 1ms, and within that ISR, increment ms_ticks.

Here are the relevant registers for Timer 0:

- TCCR0A : Timer-Counter Control Register for Timer 0 (part A)
- TCCR0B : Timer-Counter Control Register for Timer 0 (part B)
- OCR0A : Output Compare Register for Timer 0, Channel A
- TIMSK0 : Timer Interrupt Mask for Timer 0

Here are the relevant pins:

- WGM2, WGM1, WGM0 : Waveform Generation Mode Bits 210 (3 bits giving you 8 options)
- CS02, CS01, CS00 : Clock Select Bits for Timer 0 Bits 210 (3 bits giving you 8 options)
- OCIE0A : Output Compare Interrupt Enable for Timer 0, Channel A

The Waveform and Clock Select bits correspond to the TCCR0 registers (think of TCCR0A and TCCR0B as a single 16-bit register). The Interrupt Enable bit corresponds to the Interrupt Mask.

> This is an 8-bit timer

Keep in mind the formula for interrupt frequency (as shown in the datasheet):
  `16M ticks/second * 1 count/(prescaler ticks) * interrupt/(match counts) = x interrupts/second`

This is in Hertz (times/second). But you are typically given the requirements in milliseconds. For example, the interrupt to update `ms_ticks` needs to interrupt every 1 ms (i.e. has a period of 1ms). You can translate this to a frequency:
  `1 interrupt/ms * 1 second/1000 ms = x interrupts / second`

**_RECOMMENDATION_**: Setup your timer 0 to fire every 500ms and toggle an LED in the corresponding ISR as a means of debugging (first, make absolutely sure that you are operating the LED correctly!). If the LED is not toggling, just turn it on in the ISR, never off -- if it never turns on, your interrupt isn't working -- did you enable it and use `sei()`? If it does turn on but not off, maybe you don't have the match set correctly and it is firing faster than you thought. (_If you try to toggle an LED in 1ms interrupt, the LED will always seem to be on._)

Once you have your "clock" working, you can monitor ms_ticks in the main while-loop, and use it to toggle your red led at the appropriate frequency.

Define ms_ticks as a global variable, like this:
```
volatile uint32_t ms_ticks = 0;
```

If you are using it across multiple files, but you want it initialized, you will need to use the keyword `extern`. (If you want to do this but you are not sure how, ask me -- happy to explain it.)


### Timer 1 and the Yellow LED

Timer1 is ALMOST identical to the timer 0 setup. The one very critical difference is the waveform generation. There are 16 modes for the 16-bit timers, so you must look at the table provided in the chapter on 16-bit timers, not the one in the chapter on 8-bit timers.

For all of the registers and pins described above for timer 0, wherever there is a 0 in the name that corresponds to the timer (not the bit), change it to 1.

> This is a 16-bit timer, which means that the counter can count to 2^16-1!

In the ISR for timer 1, toggle the yellow LED. You don't need variables or counters for timer 1 -- when it fires, toggle yellow.

### Button A and the Green Led

Create a callback function to blink the greed led 5 times (i.e. 5 on-off toggles that will span 2 seconds). This blinking at 2.5Hz occurs INSIDE the callback function.

<hr>

Some advice:

Code a little, test a lot. Build your system 1 component at a time.

Use your on-board leds for debugging to track flow-of-control.


See the cheat sheet for info: https://github.com/lars1050/CSC395_S20/blob/master/HardwareResources/AStarCheatSheet.md

<hr>

RESOURCES:
- iom32u4.h in avr library
- AStar pinout
- Atmel Datasheet, sections 10 (I/O), 13 (8-bit counters), 14 (16-bit counters)
- AStarDataCheatSheet

##### CSC395 Spring 2022

### Lab 1 : Getting Started
> CODE DUE: Wednesday, January 26 before class <br>
> Submit via Moodle. 1 file zipped.

The purpose of this lab is to ensure that you have all equipment functional and you are ready for command-line compilation. There are various libraries that you will need to install to program using avr-gcc.

#### Assignment

1. Get everything set up as described below.
2. Test your setup with the provided main.c.
3. Add the yellow LED to the "sanity check." Note: you have to check whether the yellow led is turned on by setting the pin high or low. Just try it and see what happens. The yellow and green should both be on and off at the same time.
4. Add button C to the while loop. Remember that you have to set up the button pin as input. Whenever button C is pressed, the yellow LED should turn on.

#### Hardware Setup

You can start by installing the Arduino IDE and following the directions to add the Pololu libraries. We will not be using this much, but it is a good thing to have in case your board gets into a weird state -- you can use it to reset.

You will need the avr-gcc compiler along with the avrdude library that flashes your program to your board. You will need an editor for coding (e.g. Atom, Sublime, etc.). And if you are on Windows you will need a driver.

Directions can be found on GitHub under HardwareResources in HardwareSetup (https://github.com/lars1050/CSC395_S20/blob/master/HardwareResources/HardwareSetup.md).

Use the provided main.c file to test your system.

#### Upload to the board

> You will probably need to change the port setting in your makefile

- Plug your board into your USB
- cd labs/lab01_01_26
- at command prompt ```make```
- push the reset button twice on your board which makes the yellow LED blink slowly (it is ready for upload)
- at command prompt ```make program```

The leds will flash as it is uploading and you will get a message scroll across your screen. If it worked, you will see the green LED flash 3 times then everything goes dark. Push the A button and the green LED turns on. Release it, green is off.

<hr>

#### Important Things to know about your board.

See the cheat sheet for more info: https://github.com/lars1050/CSC395_S20/blob/master/HardwareResources/AStarCheatSheet.md

And see the datasheet on the Atmega: http://ww1.microchip.com/downloads/en/devicedoc/atmel-7766-8-bit-avr-atmega16u4-32u4_datasheet.pdf

**__Registers__**

x = A-F <br>
n = 0-7

- DDRx : Data Direction Register for Port "x"
- PORTx : Port Output for Port "x"
- PINx : Port Input for Port "x"
- PCICR : Pin Change Interrupt Control Register
- PCMSK0 : Pin Change Enable Mask for Interrupt 0

**__Pin/Bit Locations__**

- DDxn : Data Direction bit location for port "x" pin "n"
- PORTxn : Port Output location for port "x" pin "n"
- PINxn : Pin Output location for port "x" pin "n"
- PCINTn : Pin Change Interrupt Enable for pin "n"

sei() : function call to enable all interrupts

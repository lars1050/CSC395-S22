## Hardware Setup for Pololu's A-Star 32U4

These boards and the components might be easy to install and work perfectly the
first time, or it might take some doing to get all the pieces functional.
Our first day of class will be spent confirming everything is working.

The Pololu boards are built around an _Atmega 32U4_ microcontroller by _Microchip_ (formerly _Atmel_).
This processor is identical to that used in the very popular Arduino family of boards -- the Leonardo being the most similar.
You may use the Arduino libraries for the first lab, but after that, we will use
only the avr libraries. To develop code, you might be able to use the Arduino
IDE (integrated development environment), but directions will typically be given
for command line compilation using avr-gcc. Programs are written in C (not C++).

Communication with the boards in the form of uploading and user input and output
can be achieved with the single USB cable. However, you will need to install the LUFA
libraries. We will not be using LUFA for another week.

*PRIOR to CLASS*: please install the Arduino IDE, as well as the required components
for using gcc with avrdude (directions below).

_HARDWARE_: AStar 32U4 Board from Pololu. It uses the AtMega32U microprocessor.

_PROGRAMMING_: Programs are developed and compiled on your PC and the executable
is downloaded to the board through the USB connection.

IF you are using a MAC, I highly recommend using the package manager homebrew (http://brew.sh/).

Download Arduino IDE (Instructions on Pololu User Guide in “Getting Started” section)
- A-Star 32U4 User Guide: https://www.pololu.com/docs/0J66
- Arduino Download: https://www.arduino.cc/en/Main/Software

	With homebrew, try this at command prompt: `brew cask install arduino`

	(http://macappstore.org/arduino/)

- Download necessary files for C programming.
Instructions here: https://www.pololu.com/docs/0J66/4.3

With homebrew, try this:`brew cask install crosspack-avr`

It is possible to use the IDE Atmel Studio for development (Windows only). It is a little fussy, but once you get it working, it's pretty easy.
	http://www.atmel.com/tools/ATMELSTUDIO.aspx

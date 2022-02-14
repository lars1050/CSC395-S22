##### CSC395 RTES Spring 2022

#### Lab 5 : Communication and Pulse Width Modulation (PWM)
> DUE: Wednesday, Feb 23 beginning of class <br>
> Be sure to comment your code!
> Submit code via Moodle

<hr>

**__HARDWARE READINESS__**

Connect another led to General Purpose I/O (GPIO) PortB pin 6. (This is pin 10 of the Pololu board on the header.)

If you are using the serial communication, you will have to move the LED at pin 0 to another location, as pin 0 and pin 1 will be used for the serial communication dongle.

### SYSTEM REQUIREMENTS

Control the blink rate of an LED by modifying the match value of the PWM signal. Use either the buttons as control or serial communication.

### LED Configuration and Control

The PWM signal is controlled by setting up the timer that is associated with the pin that the LED is plugged into. Rather than setting it up in CTC waveform mode, you use PWM mode (probably fast pwm). The LED is connected to the OC1B pin, which means you have to set up timer 1 and the match is OCR1B. In addition to setting the waveform mode and the CS (clock select) bits, you also have to set the COM bits to non-inverted. You do NOT enable the interrupt!!

Determine how low of a waveform frequency you can achieve with fast PWM. Use these settings for blinking the led.

>If you were using timer 1 to control one of the other LEDs, then you will have to move that to another timer.

### Communication to Control LED Blink Frequency using Button C

Use the release of button C to increase the frequency up to the 100% duty cycle. Then decrement the duty cycle upon release of button C, until you reach 0%. Then back to 100%, etc. Try to make the logic/code as streamlined as possible.

### Optional Serial Communication to Control Frequency (and for debugging)

If you are on a Mac or Linux, you can try following the instructions for LUFA and see if it will work!

https://github.com/lars1050/CSC395-S22/tree/main/ExampleCode/basic-lufa

When you are ready to test it out, make and run the example https://github.com/lars1050/CSC395-S22/tree/main/ExampleCode/basic-lufa/example. You use `screen` (as indicated in the directions) to manage the communication.

Any code that you write must call `SetupHardware()` and any time that you want to send or receive data, you call `USB_Mainloop_Handler()`. Use the Makefile provided with the example.

**If you are on Windows**, you can try it, but I don't think it will work. I have hardware for serial communication using a dongle that has to be plugged in to where one of the LEDs is plugged in.

https://github.com/lars1050/CSC395-S22/tree/main/ExampleCode/basic-serial

Use the 'f' key to change the frequency of the LED controlled by the PWM. Decrease the frequency by some fixed increment when the 'f' key is pressed and increase the frequency when the 'F' key is pressed. The frequency is controlled by the DUTY CYCLE, which is defined as MATCH/TOP.

<hr>

### Optional: Vary LED Brightness

If you get that complete, you can give the user another option to control the **brightness** of the led rather than the blink frequency. To do that, change the frequency of the PWM waveform to something much faster (like 1 ms) and then adjust the duty cycle just as you had before. If you are using buttons, use the release of Button A to switch between modifying blink rate and modifying brightness. Similarly, use Button C to move through the duty cycles. Alternatively, use the 's' key to switch modes, then use the 'b' key to change the brightness.

See the cheat sheet for info: https://github.com/lars1050/CSC395-S22/blob/main/HardwareResources/AStarCheatSheet.md

<hr>

RESOURCES:
- iom32u4.h in avr library
- AStar pinout
- Atmel Datasheet, sections 10 (I/O), 13 (8-bit counters), 14 (16-bit counters)
- AStarDataCheatSheet

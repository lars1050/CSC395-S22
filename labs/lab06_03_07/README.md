##### CSC395 RTES Spring 2020

#### Lab 6 : Motor Control
#### Due Monday March 7 at end-of-day

<hr>

> Note: Please be careful with the boards and the motors. There is enough current running through these connectors that you can do damage to the board and possibly even your laptop if you short something. As you plug things in and out, the best policy is to unplug it from your laptop (pull the USB) so that your laptop is protected. Double check your wires and make sure nothing is exposed or touching.

Strive for _clean_, well documented, well organized code (i.e. professional quality code). Don't let old code hang around. Use helper functions so that your code starts to look more like the Arduino libraries, which is very user friendly. Put related code in separate files.

Well written code is part of this assignment. Really, it is always part of writing code!

#### 1. Example Code Functional

You will need to use these pins for the example code. You can change how you connect the motor, but then you have to change the code.

|Color | Function| Pin |
|-----|-----|-----|
| Red | motor power/ground | motor 2, green header (B6) |
| Black | motor power/ground | motor 2, green header (B6)
| Green | encoder GND | |
| Blue | encoder Vcc | D1 (astar 2) |
| Yellow | encoder A | B4, PCINT 4 (astar 8) |
| White | encoder B  | B5, PCINT 5 (astar 9) |

Connect the motor power/grounds to the green header at M2A and M2B (the 2 in the middle). It does not matter which way you plug it in, but it will impact the "positive" direction as measured by the encoder.

You have to use the wall adapter to give the board enough power to run the motor. There is a power switch on the side of the board. When you turn it on, that gives power to the motors from the wall adapter. A blue LED indicates power is on.

If everything is plugged in correctly, when you run the code, the motor will move in one direction increasing speed, stop then spin in the opposite direction.

__If the motor is not moving__, check that the blue power led is on (next to the power switch).

#### 2. "UI" for Entering Reference Position

Use 'a' and 'd' (or if you can figure it out, arrow keys) key presses to set the reference position of the motor. The "reference" position is the target position. You can know your current position by looking at the global variable 'global_counts_m2', which is set in the PCINT0 ISR. This is managed for you, so you should not have to modify anything in the PCINT ISR.  

Configure your system so that whenever key 'd' is pressed (or right arrow), the reference position (__a global volatile int16_t variable__) is set to +360 degrees relative to its current position, and the motor should move until it is at (or just beyond) that position. If you go to the pololu site and look up the motor #4825, you will notice "2248.86 counts per revolution" -- this is how many encoder counts there are per 360 degrees.

Configure your system so that whenever key 'a' is pressed (or left arrow), the reference position is set to -360 degrees relative to its current position and the motor should move until it is at (or just beyond) that position.

Monitor the global counts of the encoder (this is your measured position), and when you have reached your reference position, turn the motor off. Please do not run your motor at more than 60% duty cycle for now as you are getting used to your system.

__If the motor is continually running in one direction__, swap the power/grounds (you can do this by reversing the connector - you do not have to swap at the green header.)

#### 3. "UI" for Entering Speed

Use 'w' and 's' (or if you can figure it out, arrow keys) key presses to set the speed of the motor. Speed is controlled by changing the duty cycle.

Configure your system so that whenever key 'w' is pressed (or up arrow), the speed is increased by some fixed increment, up to 60% duty cylce. Do not exceed 60%. Ignore any subsequent requests to increase the speed.

Configure your system so that whenever key 's' is pressed (or down arrow), the speed is decreased by some fixed increment, down to 0% duty cycle. Do not go negative on the speed.


#### 4. "UI" with Dials

Rather than the keys on the keyboard, use a potentiometer to control the speed of the motor. There is basic-analog code posted in the Example Code with directions. Here is a little blurb on potentiometers https://www.arduino.cc/en/tutorial/potentiometer.

Note that you will be _polling_ the potentiometer for changes, but you should probably ignore minor fluctuations. Perhaps change the speed when you see a change of 3-5 from the last time you changed the speed. OR, keep a buffer and look for a consistent change across polling cycles.

As a user, stick to either the keyboard or the potentiometer for controlling speed. As for the assignment, keep them both operational so I can see them working.

Again, do not let your motor run faster than 60% duty cycle for this exercise.

> Once you have the above complete and operational, take a snapshot of your code before moving on to the next section. Perhaps push a version up to your GitHub account or make a copy of the folder.

#### 5. Data Structures and "UI"

Create an FIFO buffer to store reference positions that are requested by the user. Accept up to 5 integers/key presses at any given time (positive and negative). If the user enters an integer, add it to the buffer at the end. If the user presses 'd' or 'a', add +360 or -360, respectively to the buffer at the end. Interpret input as a *relative* reference position with respect to the current position. Try to pause for a second in between each reference position. Print a statement at the time the ring buffer becomes full or empty.

In main, constantly check if there are more reference position requests in the buffer. Note that you really want a _ring_ buffer. Remember that you can use "head" and "tail" markers in an array to accomplish this, and it can make the code a little easier if you have one empty slot between the head and tail.

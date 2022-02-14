# Basic USB-Serial Communication

(Adapted From Jonathan Walz - SENG5831 Spring 2016)

This is a basic example that enables serial communication with stdio.

(Note: I've only tested this on a mac for an ATmega32U4 for seng-5831S16)

# Download VirtualSerial and Set-Up Path

-. <del>Download the [current VirtualSerial.tgz] (https://github.umn.edu/jpwalz/basic-lufa-serial/releases/latest) archive.</del>

-. <del>Extract the library somewhere on your machine and take note of the path. This will be assigned to VIRTUAL_SERIAL_PATH.<del>
1. Download the VirtualSerial folder (https://github.com/lars1050/CSC395_S20/tree/master/ExampleCode/VirtualSerial) and place somewhere memorable.
2. At the OS command prompt, add the path that corresponds to your VirtualSerial folder location, for example:
  ```export VIRTUAL_SERIAL_PATH=/Users/amylarson/RTES/VirtualSerial```
3. Use stdin/stdout with the [stdio] (http://www.nongnu.org/avr-libc/user-manual/group__avr__stdio.html) library.


# Download LUFA, Set-Up Path, make library

-. <del>Download [LUFA] (http://www.fourwalledcubicle.com/LUFA.php). ([Direct Download] (http://www.github.com/abcminiuser/lufa/archive/LUFA-151115.zip))</del>
-. <del>Extract LUFA somewhere on your machine and take note of the path. This will be assigned to LUFA_PATH.</del>
1. Download the built [LUFA] folder https://github.com/lars1050/CSC395_S20/tree/master/ExampleCode/lufa-LUFA-151115 and place somewhere memorable.
2. At the OS command prompt, add that path that corresponds to your LUFA folder, for example:
  ```export LUFA_PATH=/Users/amylarson/RTES/lufa-LUFA-151115/LUFA/```
3. Move into the 'basic-lufa' directory.
4. `make`

> Every time you open a new terminal, you have to again "export LUFA_PATH..." and the VIRTUAL_PATH. The code will compile even if you do not have the path set (it does display a warning), so it seems like everything worked in compilation, but it will not communicate with the board!

# Connect

1. Open a new terminal window (MAC and Linux only).
2. Open connection using `screen /dev/tty.usbmodem1411 -f 9600`.
3. Test connection: `return` should display Hello message to the screen.
4. To exit connection: `ctrl-a` then `ctrl-\` then `y` (There must be an easier way :-)

If on Windows, need to use PutTy or something like it to make the connection.

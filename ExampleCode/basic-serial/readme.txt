This sample program makes use of the Adafruit USB to TTL Serial Cable (product id 954). In combination with a serial comm terminal (e.g. PuTtY for Windows, screen for Mac, ?? for Linux - maybe PuTtY as well), you can send and receive text between the AStar32U4 and your laptop/PC.

There are 4 wires extending from the USB device.
 
Red/Black for power/ground. Power you don't since you are 
powering the board through the USB.

Ground should be plugged in to the ground on the board.

White is transmit (TX) to the terminal, thus should be connected to the
Arduino pin 1, a.k.a. PortD Pin3.

Green is receive (RX) from the terminal, thus should be connected to the
Arduino pin 0, a.k.a. PortD Pin2. 

On MAC:
   Flash the program.
   Open terminal.
   Find Port (ls /dev/tty.*)
   screen /dev/tty.usbserial 38400
   "ctrl+a" "ctrl+\" "y"

38400 is the baud rate.

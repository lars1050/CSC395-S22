#ifdef VIRTUAL_SERIAL
#include <VirtualSerial.h>
#else
#warning VirtualSerial not defined, USB IO may not work
#define SetupHardware();
#define USB_Mainloop_Handler();
#endif

#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h>

void init() {
    SetupHardware(); //This setups the USB hardware and stdio
    sei(); //Enables interrupts - needed for USB
}

int main()
{
    char c;
    init();

    while(1)
    {
        USB_Mainloop_Handler(); //Handles USB communication

        if ((c=fgetc(stdin)) != EOF ) {
            printf("Hello! Hit any key, including %c to see this again!\r\n", c);
        }
    }
}

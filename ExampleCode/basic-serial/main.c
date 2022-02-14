
#define F_CPU 16000000ul	// required for _delay_ms()
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Length of output bufer used for serial communication
#define OUTPUT_BUFFER_LENGTH 300
//Length of input buffer used to store user configuration option
#define RECEIVE_BUFFER_LENGTH 50
//Global constant string specifying menu options:
const char *menu = "MENU\n\r";

//Serial communication output buffer
static volatile char output_buffer[OUTPUT_BUFFER_LENGTH];
static volatile char recv_buffer[RECEIVE_BUFFER_LENGTH];
volatile uint8_t recv_buffer_ptr = 0;
volatile unsigned int user_command_ready = 0;
volatile unsigned int *user_token;

void handleInput();

//Function that takes care of configurating the serial communication
void setupUART(void) {
	UBRR1 = (F_CPU/9600/16) - 1;

	UCSR1C |= (1 << UCSZ11) | (1 << UCSZ10);		// 8 bit char size
	UCSR1B |= (1 << TXEN1);		// enable transmit

	UCSR1B |= (1 << RXEN1);     // enable receive
	UCSR1B |= (1 << RXCIE1);     // enable interrupt on data receipt
}

//ISR that is called whenever there is data to receive
ISR(USART1_RX_vect) {

		// BE CAREFUL with this. It is an ISR and will interfere with your
		// PCINT if it takes too long.

    uint8_t ch = UDR1;

    //Is user pressed 'Enter' it means that the user has finished typing the command
    if (ch == 13 || (recv_buffer_ptr >= RECEIVE_BUFFER_LENGTH)) {
        user_command_ready = 1;
    }

    //User deletes a character
    else if (ch == 8) {
        if (recv_buffer_ptr >= 1)
            --recv_buffer_ptr;
        recv_buffer[recv_buffer_ptr] = 0;
    }

    //Only store alphanumeric symbols, space, the dot, plus and minus sign
    else if
		( (ch == ' ') || (ch == '.') || (ch == '+') || (ch == '-') ||
		((ch >= '0') && (ch <= '9')) ||
		((ch >= 'A') && (ch <= 'Z')) ||
		((ch >= 'a') && (ch <= 'z')) ) {
        recv_buffer[recv_buffer_ptr] = ch;
        ++recv_buffer_ptr;
    }
}

void sendChar(char c) {
	while((UCSR1A & (1<<UDRE1)) == 0);	// wait while data register is NOT empty
	UDR1 = c;
}

void sendString(char *s) {
	while(*s != 0x00) {
		sendChar(*s);
		s++;
	}
	sendChar('\r');
}

int main(void) {
	USBCON = 0;

	DDRC |= (1<<PORTC7);
	int i;
	for (i=0;i<10;i++)
	  _delay_ms(1000);


	//DDRD = 0x
  setupUART();
	sendString("Here we go!\r\n ");
	sei();
  for(;;) {
		sendChar(65);
		PORTC |= (1<<PORTC7);
		_delay_ms(300);

		PORTC &= ~(1<<PORTC7);
		_delay_ms(300);

		if (user_command_ready) {
		  sendChar('B');
			handleInput();
		}
	}
	return 0;
}

#define printMenu() sendString("P, p, D, d, F #, R #, S, Z, V\r\n");

//Function that parses the commands the user inputs into the serial console.
void handleInput() {

		// This function is called from main, but "released" in the ISR.
		// The ISR sets user_command_ready=1, main watches for the flag.

		char command;
		int value;
		char fGood = 1;

		// provides feedback to the terminal
		char outputBuffer[50] = "\r\n";

		// only accepting a command character with optional value
		sscanf((char *)recv_buffer,"%c %d",&command, &value);

		// Make this into whatever makes sense for you
		switch(command) {
			case 'P':
				// add your function here
				sprintf( outputBuffer,"Increasing kP.\r\n");
			case 'p':
				// add your function here
				sprintf( outputBuffer,"Decreasing kP.\r\n");
				break;
			case 'D':
			// add your function here
				sprintf( outputBuffer,"Increasing kD.\r\n");
				break;
			case 'd':
			// add your function here
				sprintf( outputBuffer,"Decreasing kD.\r\n");
				break;
			case 'F':
			// add your function here
				sprintf( outputBuffer,"Forward %d deg.\r\n",value);
				break;
			case 'R':
				// add your function here
				sprintf( outputBuffer,"Reverse %d deg.\n",value);
				break;
			case 'S':
			case 's':
			// add your function here
				sprintf( outputBuffer,"Stop Motor.\r\n");
				break;
			case 'Z':
			case 'z':
				// add your function here
				sprintf( outputBuffer,"Zero Encoder.\r\n");
				break;
			case 'V':
			case 'v':
				// add your function here
				sprintf( outputBuffer,"View Data.\r\n");
				break;
			default:
				fGood = 0;
				printMenu();
		}

		// reset the buffer
    recv_buffer_ptr = 0;
    memset((char *)recv_buffer, 0, RECEIVE_BUFFER_LENGTH);

		// feedback to terminal
		if (fGood) sendString(outputBuffer);

		// set flag that ready for more
		user_command_ready = 0;
}

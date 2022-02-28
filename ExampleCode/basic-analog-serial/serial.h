#ifndef F_CPU
#define F_CPU 16000000
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Length of output bufer used for serial communication
#define OUTPUT_BUFFER_LENGTH 300
//Length of input buffer used to store user configuration option
#define RECEIVE_BUFFER_LENGTH 50
//Global constant string specifying menu options:
//const char *menu = "MENU\n\r";

//Serial communication output buffer
static volatile char output_buffer[OUTPUT_BUFFER_LENGTH];
static volatile char recv_buffer[RECEIVE_BUFFER_LENGTH];
volatile uint8_t recv_buffer_ptr;
volatile unsigned int user_command_ready;
volatile unsigned int *user_token;

void handleInput(void);
void setupUART(void);
void sendChar(char c);
void sendString(char *s);

#define printMenu() sendString("P, p, D, d, F #, R #, S, Z, V\r\n");

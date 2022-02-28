#ifndef COMMON_H_
#define COMMON_H_

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>

#define ERROR -1

#define SET_BIT(port, pin) (port |= (1<<pin))
#define CLEAR_BIT(port, pin) (port &= ~(1<<pin))
#define TOGGLE_BIT(port, pin) (port ^= (1<<pin))

#define CONFIG_OUTPUT(port, pin) (SET_BIT(port,pin))
#define CONFIG_INPUT(port,pin) (CLEAR_BIT(port,pin))

typedef struct {
  volatile uint8_t * ddr;
  volatile uint8_t * port;
  uint8_t pin;
  volatile uint8_t * portin;
} IO_struct;

#endif

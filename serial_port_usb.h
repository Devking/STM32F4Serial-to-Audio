#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#include <stdint.h>


// initialize serial port via micro USB on STM32F4 Discovery Board
void init_serial_port_usb(void); 

// returns 1 if the serial port connected (via micro USB)
uint8_t is_serial_port_usb_connected(void); 

// read a byte from the serial port (via micro USB); byte is returned in *c; function returns 1 if successfully read, 0 if not
uint8_t read_serial_usb_byte(uint8_t *c); 

// write a byte to the serial port (via micro USB)
void write_serial_usb_byte(uint8_t a);

#endif

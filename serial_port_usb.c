// serial port functions (over micro USB on STM32F4 Discovery Board) 
// utilizing USB VCP library from: http://stm32f4-discovery.com/2014/08/library-24-virtual-com-port-vcp-stm32f4xx/
// use STM32 Virtual COM Port Driver (http://www.st.com/web/en/catalog/tools/PF257938#) on computer to connect to serial port over USB connection

#include "serial_port_usb.h"
#include "usb_cdc_device/tm_stm32f4_usb_vcp.h"

// initialize serial port via micro USB on STM32F4 Discovery Board
void init_serial_port_usb()
{
		TM_USB_VCP_Init();
}

// returns 1 if the serial port connected (via micro USB)
uint8_t is_serial_port_usb_connected()
{
	return (TM_USB_VCP_GetStatus() == TM_USB_VCP_CONNECTED);	
}

// read a byte from the serial port (via micro USB); byte is returned in *c; function returns 1 if successfully read, 0 if not successfully read
uint8_t read_serial_usb_byte(uint8_t *c) 
{
	return (TM_USB_VCP_Getc(c) == TM_USB_VCP_DATA_OK);
}

// write a byte to the serial port (via micro USB)
void write_serial_usb_byte(uint8_t a) 
{
	TM_USB_VCP_Putc(a);
}

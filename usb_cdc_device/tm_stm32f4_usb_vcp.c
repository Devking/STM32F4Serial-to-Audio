/**	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2014
 * | 
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |  
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * | 
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 */
#include "tm_stm32f4_usb_vcp.h"
#include "usbd_usr.h"

uint8_t TM_INT_USB_VCP_ReceiveBuffer[USB_VCP_RECEIVE_BUFFER_LENGTH];
uint32_t tm_int_usb_vcp_buf_in, tm_int_usb_vcp_buf_out, tm_int_usb_vcp_buf_num;
extern TM_USB_VCP_Result TM_USB_VCP_INT_Status;
uint8_t TM_USB_VCP_INT_Init = 0;

USB_OTG_CORE_HANDLE	USB_OTG_dev;

/* USB VCP Internal receive buffer */
extern uint8_t TM_INT_USB_VCP_ReceiveBuffer[USB_VCP_RECEIVE_BUFFER_LENGTH];


TM_USB_VCP_Result TM_USB_VCP_Init(void) {
	/* Initialize USB */
	USBD_Init(	&USB_OTG_dev,
#ifdef USE_USB_OTG_FS
				USB_OTG_FS_CORE_ID,
#else
				USB_OTG_HS_CORE_ID,
#endif
				&USR_desc, 
				&USBD_CDC_cb, 
				&USR_cb);
	
	/* Reset buffer counters */
	tm_int_usb_vcp_buf_in = 0;
	tm_int_usb_vcp_buf_out = 0;
	tm_int_usb_vcp_buf_num = 0;
	
	/* Initialized */
	TM_USB_VCP_INT_Init = 1;
	
	/* Return OK */
	return TM_USB_VCP_OK;
}

TM_USB_VCP_Result TM_USB_VCP_Getc(uint8_t* c) {
	/* Any data in buffer */
	if (tm_int_usb_vcp_buf_num > 0) {
		/* Check overflow */
		if (tm_int_usb_vcp_buf_out >= USB_VCP_RECEIVE_BUFFER_LENGTH) {
			tm_int_usb_vcp_buf_out = 0;
		}
		*c = TM_INT_USB_VCP_ReceiveBuffer[tm_int_usb_vcp_buf_out];
		TM_INT_USB_VCP_ReceiveBuffer[tm_int_usb_vcp_buf_out] = 0;
		
		/* Set counters */
		tm_int_usb_vcp_buf_out++;
		tm_int_usb_vcp_buf_num--;
		
		/* Data OK */
		return TM_USB_VCP_DATA_OK;
	}
	*c = 0;
	/* Data not ready */
	return TM_USB_VCP_DATA_EMPTY;
}

TM_USB_VCP_Result TM_USB_VCP_Gets(char* buffer, uint8_t bufsize) {
	uint16_t i = 0;                             
	uint8_t eol = 0;
	uint8_t c;
	if (TM_USB_VCP_Getc(&c) != TM_USB_VCP_DATA_OK) {
		return TM_USB_VCP_DATA_EMPTY;
	}
	if (bufsize > 0) {
		buffer[i++] = (char) c;
		while (!eol) {
			while (TM_USB_VCP_Getc(&c) != TM_USB_VCP_DATA_OK);
			buffer[i] = (char) c;   
			if (buffer[i] == '\n') {
				eol = 1;                
			} else {            
				if (i < (bufsize - 1)) {
					i++; 	
				}
			}
		}
		/* Add zero to the end of string */
		buffer[i] = 0;               
	}
	/* Data ok */
	return TM_USB_VCP_DATA_OK;
}

TM_USB_VCP_Result TM_USB_VCP_Putc(volatile char c) {
	uint8_t ce = (uint8_t)c;
	/* Send data over USB */
	VCP_DataTx(&ce, 1);
	
	/* Return OK */
	return TM_USB_VCP_OK;
}

TM_USB_VCP_Result TM_USB_VCP_Puts(char* str) {
	while (*str) {
		TM_USB_VCP_Putc(*str++);
	}
	
	/* Return OK */
	return TM_USB_VCP_OK;
}

TM_USB_VCP_Result TM_INT_USB_VCP_AddReceived(uint8_t c) {
	/* Still available data in buffer */
	if (tm_int_usb_vcp_buf_num < USB_VCP_RECEIVE_BUFFER_LENGTH) {
		/* Check for overflow */
		if (tm_int_usb_vcp_buf_in >= USB_VCP_RECEIVE_BUFFER_LENGTH) {
			tm_int_usb_vcp_buf_in = 0;
		}
		/* Add character to buffer */
		TM_INT_USB_VCP_ReceiveBuffer[tm_int_usb_vcp_buf_in] = c;
		/* Increase counters */
		tm_int_usb_vcp_buf_in++;
		tm_int_usb_vcp_buf_num++;
		
		/* Return OK */
		return TM_USB_VCP_OK;
	}
	/* Return Buffer full */
	return TM_USB_VCP_RECEIVE_BUFFER_FULL;
}

TM_USB_VCP_Result TM_USB_VCP_GetStatus(void) {
	if (TM_USB_VCP_INT_Init) {
		return TM_USB_VCP_INT_Status;
	}
	return TM_USB_VCP_ERROR;
}


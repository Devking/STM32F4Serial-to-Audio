//////////////////////////////////////////////////////////////
//                                                          //
// Project Completed By Wells Lucas Santo                   //
// Found at wls254@nyu.edu and GitHub user: Devking         //
//                                                          //
// This project was completed for educational purposes, as  //
// my final semester project for the NYU Polytechnic course //
// EE4144 (Embedded Systems Design). If you wish to use any //
// of this code for your own projects, please give credit!  //
//                                                          //
//////////////////////////////////////////////////////////////
//                                                          //
// This project utilizes code from two outside sources:     //
//                                                          //
// (1) Professor Prashanth Krishnamurthy from NYU SoE       //
//     who provided the majority of the code to set up the  //
//     the STM32F4 board and to program the board for a     //
//     serial port interface.                               //
//     (source: http://crrl.poly.edu/EE4144/)               //
//                                                          //
// (2) Andreas Finkelmeyer from his online blog where he    //
//     wrote, described, and uploaded code to initialize    //
//     the STM32F4 Discovery Board for audio output using   //
//     I2S communication to send data to the on-board DAC.  //
//     (source: http://www.mind-dump.net/                   //
//             configuring-the-stm32f4-discovery-for-audio) //
//                                                          //
//////////////////////////////////////////////////////////////
//                                                          //
// This project is meant for use with the Keil uVision 5    //
// MDK and the STM32F4 Discovery Board.                     //
//                                                          //
//////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#define ARM_MATH_CM4
#include <arm_math.h>
#include <math.h>
#include "stm32f4xx.h"
#include "serial_port_usb.h"
#include "speaker.h"
#include "macros.h"

volatile uint32_t msTicks = 0;

// SysTick Handler (every time the interrupt occurs, this is called)
void SysTick_Handler(void){ msTicks++; }

// initialize the system tick 
void InitSystick(void){
	SystemCoreClockUpdate();
	// division occurs in terms of seconds... divide by 1000 to get ms, for example
  if (SysTick_Config(SystemCoreClock / 10000)) { while (1); } // update every 0.0001 s, aka 10kHz
}

int32_t main(void){

	// This is the standard initialization stage for our board
	SystemInit();
	InitSystick();
	init_serial_port_usb();  // initialize serial port (via micro USB)
	init_speaker();  // initialize speaker (audio out)
	
	int16_t audio_sample;	// to be sent to the speaker
	float audio_freq = 0; // frequency of sound to send, initialized to 0
	
	int loudness = 250; // amplitude of sound
	// 1000 allows sound to be heard out of headphones
	// 500 allows sound to be loud w/ headphones on, quiet with headphones off
	// the smaller this is, the quieter the sound
	// the smaller this is, the less the "jumps" when you change notes
	
	float readTimeDelay = 0.04; // only check serial port every 10 ms to reduce distortion
	// will not decrease distortion if you increase after a point
	// will force serial byte buffer to flood if number is too high
	// make sure to make less than 40 ms so that the serial byte buffer does not overload
	
	float noteTimeDelay = 0.25; // single notes have this duration (decrease to allow more differentiation between notes)
	// 100 ms makes it short and too staccatto, 400 ms makes it too long
	// anything under 250 ms will be buggy and interpret the first note and the held note incorrectly
	// if noteTimeDelay is too long, then it will be hard to rapidly press a note
	// thus make sure to make between 250 ms and 400 ms

	// note that you always want noteTimeDelay > readTimeDelay
	
	// we want to keep track of the last time a note was pressed
	// this is used in conjunction with the 'noteTimeDelay' variable
	float last_freq_changed_time = -1;

	// we want to keep track of the last time we check the serial buffer
	// this is used in conjunction with the 'readtimeDelay' variable
	float last_read_time = -1;

	// we want to keep track of the last note we played
	// this is used to allow for a smoother output tone when a key is held down
	// rather than registering it as a collection of separate keypresses
	// the value '255' is chosen because it is an ASCII value that we do not
	// use to output a sound with using the keyboard
	uint8_t lastnote = 255;

	uint8_t c;	// to store the current value sent by the serial port
	
	// pressing the button quickly is ok but holding the button for a long time is not... why?
	
	while (1) {
		float t = msTicks / 10000.0; // calculate time -- SysTick is configured for 0.1 ms
		
		// only check the serial port every "readTimeDelay" to allow more uniform sinusoid output
		if (t > (last_read_time + readTimeDelay)) {
			if (is_serial_port_usb_connected()) { // if serial port connected through USB micro cable
				// this occurs continuously in real time
				
				// we have to do this check every readTimeDelay
				if (read_serial_usb_byte(&c)) { // read a byte

					if (lastnote == c) { // the same note is still being pressed
						// allows the system to know that a button is being held down and thus we should lengthen the note duration
						last_freq_changed_time = t;
					} else { // a new note has been pressed
						
						switch (c) {
								case 'a': write_serial_usb_byte(c); audio_freq = C5; last_freq_changed_time = t; lastnote = c; break;
								case 'w': write_serial_usb_byte(c); audio_freq = CS5; last_freq_changed_time = t; lastnote = c;  break;
								case 's': write_serial_usb_byte(c); audio_freq = D5; last_freq_changed_time = t; lastnote = c;  break;
								case 'e': write_serial_usb_byte(c); audio_freq = DS5; last_freq_changed_time = t; lastnote = c;  break;
								case 'd': write_serial_usb_byte(c); audio_freq = E5; last_freq_changed_time = t; lastnote = c;  break;
								case 'f': write_serial_usb_byte(c); audio_freq = F5; last_freq_changed_time = t; lastnote = c; break;
								case 't': write_serial_usb_byte(c); audio_freq = FS5; last_freq_changed_time = t; lastnote = c; break;
								case 'g': write_serial_usb_byte(c); audio_freq = G5; last_freq_changed_time = t; lastnote = c; break;
								case 'y': write_serial_usb_byte(c); audio_freq = GS5; last_freq_changed_time = t; lastnote = c; break;
								case 'h': write_serial_usb_byte(c); audio_freq = A5; last_freq_changed_time = t; lastnote = c;  break;
								case 'u': write_serial_usb_byte(c); audio_freq = AS5; last_freq_changed_time = t; lastnote = c; break;
								case 'j': write_serial_usb_byte(c); audio_freq = B5; last_freq_changed_time = t; lastnote = c;  break;
								case 'k': write_serial_usb_byte(c); audio_freq = C6; last_freq_changed_time = t; lastnote = c; break;
								case 'A': write_serial_usb_byte(c); audio_freq = C6; last_freq_changed_time = t; lastnote = c; break;
								case 'W': write_serial_usb_byte(c); audio_freq = CS6; last_freq_changed_time = t; lastnote = c; break;
								case 'S': write_serial_usb_byte(c); audio_freq = D6; last_freq_changed_time = t; lastnote = c; break;
								case 'E': write_serial_usb_byte(c); audio_freq = DS6; last_freq_changed_time = t; lastnote = c; break;
								case 'D': write_serial_usb_byte(c); audio_freq = E6; last_freq_changed_time = t; lastnote = c; break;
								case 'F': write_serial_usb_byte(c); audio_freq = F6; last_freq_changed_time = t; lastnote = c; break;
								case 'T': write_serial_usb_byte(c); audio_freq = FS6; last_freq_changed_time = t; lastnote = c; break;
								case 'G': write_serial_usb_byte(c); audio_freq = G6; last_freq_changed_time = t; lastnote = c; break;
								case 'Y': write_serial_usb_byte(c); audio_freq = GS6; last_freq_changed_time = t; lastnote = c; break;
								case 'H': write_serial_usb_byte(c); audio_freq = A6; last_freq_changed_time = t; lastnote = c; break;
								case 'U': write_serial_usb_byte(c); audio_freq = AS6; last_freq_changed_time = t; lastnote = c; break;
								case 'J': write_serial_usb_byte(c); audio_freq = B6; last_freq_changed_time = t; lastnote = c; break;
								case 'K': write_serial_usb_byte(c); audio_freq = C7; last_freq_changed_time = t; lastnote = c; break;
							}
					
						} 
					
						
				} else { // if no byte is read from the serial port
					// this enforces the duration of a sound to noteTimeDelay length
					// will also skip this if the system is already not outputting sound
					
					if (t > (last_freq_changed_time + noteTimeDelay) && audio_freq != 0) {
						audio_freq = 0;
						last_freq_changed_time = t;
						lastnote = 255;
					}
					
				} // end check byte
			} // end check usb
			last_read_time = t;
		} // end last read if statementA

		
		// you must ALWAYS send this output to create a full, proper sine wave
		// the goal is to get this to be output at uniform intervals
		// what happens if a note is changed? will disrupt the sine wave...
		audio_sample = (int16_t) (loudness * arm_sin_f32(audio_freq*msTicks/10000)); // calculate one sample for the speaker
		// audio_sample += (int16_t) (loudness/2 * arm_sin_f32(audio_freq*2*msTicks/10000)); // send harmonic
		send_to_speaker(audio_sample);	// send one single audio TIME sample to the audio output
		
		//if (msTicks > ) msTicks = 0;
	}
	
}

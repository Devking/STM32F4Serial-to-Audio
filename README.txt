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

---------------------------------------------------------------------------------------------
Below, explanation provided by Professor Krishnamurthy on establishing the serial connection:
---------------------------------------------------------------------------------------------

The STM32F4 Discovery Board USB micro connector can be used to implement a serial port to communicate with another microcontroller or a computer. To connect to a computer, you can use a USB micro to regular USB 2.0 cable (this is the typical cable used in mobile phone chargers). This cable would be connected to the USB micro port on the STM32F4 Discovery board (this port is on the opposite side of the board from the programming connector). The code in this Keil project utilizes the library from http://stm32f4-discovery.com/2014/08/library-24-virtual-com-port-vcp-stm32f4xx/ to make a serial port connection through the micro USB port.  

On the computer side, you will need to install the STM32 virtual COM port driver from http://www.st.com/web/en/catalog/tools/PF257938# .  After this driver is installed, you should see a serial port (windows device manager calls this a COM port) on the computer when you run this code on the Discovery board (you might sometimes need to unplug and replug the USB cable to see the COM port). 

You can use a program such as Putty (http://www.chiark.greenend.org.uk/~sgtatham/putty/download.html) to connect to the serial (COM) port on the computer side (in Putty, select the Serial option under Connection type in the main window of Putty and change COM1 under Serial line to the COM port number corresponding to the STM32 USB connection; you can see the COM port number in the device manager in Windows).

The example in this Keil project echoes bytes received from the serial port back to the serial port. When you run this on your Discovery board and connect from the computer using Putty, you should see that any characters that you type into Putty will be echoed back (i.e., will appear in the Putty terminal).
 

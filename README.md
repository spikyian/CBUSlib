# CBUSlib
## C library files for CBUS Introduction ##
These files form an 'empty' CBUS module. It will conform to the CBUS specification but provides no module specific functionality.

The files are not strictly a library but a set of source files which are added to the application code to build the firmware for a module.

## Requirements ##
The files currently support:
* MPLAB-X
* XC8 compiler
* PIC18F25K80

## Usage ##
To use these files clone this repo into your local git directory.
Create a new project within MPLAB-X selecting:
* Standalone Microchip project
* Advanced 8-bit MCUs, PIC18F25K80
* PICkit3
* XC8 toolchain
* Specify your project name
* Select the Source folder, right click and select "Add Existing Items from Folders". Select CBUSlib in your git folder and ensure .c file type is selected. 
* Select the Header folder, right click and select "Add Existing Items from Folders". Select CBUSlib in your git folder and ensure .h file type is selected. 
* Rename myModule.c to a sensible name for your module.
* edit module.h with your module specifc details.

## Release Notes ##
Currently The BlinkLED does not flash the LED when processing a CBUS message.

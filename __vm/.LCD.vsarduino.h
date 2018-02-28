/* 
	Editor: http://www.visualmicro.com
			visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
			the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
			all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
			note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	Hardware: Teensy 3.2 / 3.1, Platform=teensy3, Package=teensy
=======
	Hardware: Arduino Pro or Pro Mini w/ ATmega328 (5V, 16 MHz), Platform=avr, Package=arduino
>>>>>>> Added Shift register support for LED bars
=======
	Hardware: Arduino Pro or Pro Mini w/ ATmega328P (5V, 16 MHz), Platform=avr, Package=arduino
>>>>>>> Fixed wrong order on shift registers and added warning display on values
=======
	Hardware: Arduino Pro or Pro Mini w/ ATmega328P (5V, 16 MHz), Platform=avr, Package=arduino
>>>>>>> Updated info
=======
	Hardware: Arduino Pro or Pro Mini w/ ATmega328 (5V, 16 MHz), Platform=avr, Package=arduino
>>>>>>> Added Shift register support for LED bars
*/

#if defined(_VMICRO_INTELLISENSE)

#ifndef _VSARDUINO_H_
#define _VSARDUINO_H_
<<<<<<< HEAD
<<<<<<< HEAD
#define __HARDWARE_MK20dx256__
#define __HARDWARE_MK20DX256__
#define __MK20DX256__
#define TEENSYDUINO 141
#define ARDUINO 10805
#define F_CPU 96000000
#define USB_SERIAL
#define LAYOUT_US_ENGLISH
=======
=======
>>>>>>> Updated info
#define __AVR_ATmega328p__
#define __AVR_ATmega328P__
#define F_CPU 16000000L
#define ARDUINO 106011
#define ARDUINO_AVR_PRO
#define ARDUINO_ARCH_AVR
<<<<<<< HEAD
>>>>>>> Added Shift register support for LED bars
#define __cplusplus 201103L
#define __arm__
#define __ARM__
#define __extension__
#define  __attribute__(x)
typedef void *__builtin_va_list;
#define __extension__
#define __ATTR_PURE__
#define __ATTR_CONST__
#define __inline__
#define __asm__ 
#define __volatile__
#define _HAVE_STDC

#define NEW_H


<<<<<<< HEAD
#include <arduino.h>
#define abs(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

#define __arm__
#define __ARM__
#define __extension__
#define  __attribute__(x)
typedef void *__builtin_va_list;
<<<<<<< HEAD
#define __extension__
#define __ATTR_PURE__
#define __ATTR_CONST__
#define __inline__
#define __asm__ 
#define __volatile__
=======
#include <Arduino.h>
=======
=======
#define __cplusplus 201103L
#define __AVR__
#define __inline__
#define __asm__(...)
#define __extension__
#define __inline__
#define __volatile__
#define GCC_VERSION 40902

#define __cplusplus 201103L
#undef __cplusplus
#define __cplusplus 201103L

#define volatile(va_arg) 
#define _CONST
#define __builtin_va_start
#define __builtin_va_end
#define __attribute__(...)
#define NOINLINE __attribute__((noinline))
#define prog_void
#define PGM_VOID_P int


#ifndef __builtin_constant_p
	#define __builtin_constant_p __attribute__((__const__))
#endif
#ifndef __builtin_strlen
	#define __builtin_strlen  __attribute__((__const__))
#endif


#define NEW_H
typedef void *__builtin_va_list;
//extern "C" void __cxa_pure_virtual() {;}

typedef int div_t;
typedef int ldiv_t;


typedef void *__builtin_va_list;
>>>>>>> Updated info
//extern "C" void __cxa_pure_virtual() {;}



<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
#include <arduino.h>
>>>>>>> Fixed wrong order on shift registers and added warning display on values
#include <pins_arduino.h> 
//#undef F
//#define F(string_literal) ((const PROGMEM char *)(string_literal))
#undef PSTR
#define PSTR(string_literal) ((const PROGMEM char *)(string_literal))
>>>>>>> Added Shift register support for LED bars

#define __disable_irq() __asm__ volatile("");
#define __enable_irq()	__asm__ volatile("");


#define NEW_H
#include "LCD.ino"
=======
#include <Arduino.h>
=======
#include <arduino.h>
>>>>>>> Fixed char overflow after IDE update
=======
#include <Arduino.h>
>>>>>>> Added Shift register support for LED bars
#include <pins_arduino.h> 
#undef F
#define F(string_literal) ((const PROGMEM char *)(string_literal))
#undef PSTR
#define PSTR(string_literal) ((const PROGMEM char *)(string_literal))


#define pgm_read_byte(address_short) uint8_t() 
#define pgm_read_word(address_short) uint16_t() 
#define pgm_read_dword(address_short) uint32_t()
#define pgm_read_float(address_short) float()
#define pgm_read_ptr(address_short)   short()

<<<<<<< HEAD
#include "SleipnirLCD.ino"
>>>>>>> Updated info
=======
#include "LCD.ino"
>>>>>>> Fixed char overflow after IDE update
#endif
#endif

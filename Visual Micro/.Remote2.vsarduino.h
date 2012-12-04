//Board = Arduino Uno
#define ARDUINO 102
#define __AVR_ATmega328P__
#define F_CPU 16000000L
#define __AVR__
#define __cplusplus
#define __attribute__(x)
#define __inline__
#define __asm__(x)
#define __extension__
#define __ATTR_PURE__
#define __ATTR_CONST__
#define __inline__
#define __asm__ 
#define __volatile__
#define __builtin_va_list
#define __builtin_va_start
#define __builtin_va_end
#define __DOXYGEN__
#define prog_void
#define PGM_VOID_P int
#define NOINLINE __attribute__((noinline))

typedef unsigned char byte;
extern "C" void __cxa_pure_virtual() {}

//already defined in arduno.h
//already defined in arduno.h
void sendRawPulses(uint16_t *pulses, uint16_t pulseLength);
void getButton();
void intCode();
void goToSleep();

#include "C:\arduino\hardware\arduino\variants\standard\pins_arduino.h" 
#include "C:\arduino\hardware\arduino\cores\arduino\arduino.h"
#include "C:\Users\Josh\Documents\Arduino\Remote2\Remote2.ino"
#include "C:\Users\Josh\Documents\Arduino\Remote2\Codes.h"

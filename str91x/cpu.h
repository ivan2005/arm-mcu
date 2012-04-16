/* ARM microcontroller device specific definitions and header files */

// $Id$

#ifndef _CPU_H
#define _CPU_H

#include <_ansi.h>
#include <arm.h>
#include <device.h>
#include <interrupt.h>
#include <leds.h>
#include <serial.h>

#include <usb_serial/usb_serial.h>
#include <91x_lib.h>
#include <91x_it.h>

#ifndef DEFAULT_CPU_FREQ
#define DEFAULT_CPU_FREQ	48000000
#endif

#ifndef CONSOLE_PORT
#define CONSOLE_PORT		"com1:115200,n,8,1"
#endif

_BEGIN_STD_C

extern void cpu_init(unsigned long int frequency);

extern unsigned long int CPUFREQ;

_END_STD_C
#endif

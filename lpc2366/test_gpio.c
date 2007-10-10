/* Simple bit twiddler test program */

// $Id: test_gpio.c,v 1.4 2007-10-10 03:24:05 cvs Exp $

#include <cpu.h>

int main(void)
{
  unsigned long int i;

  cpu_init(48000000);

/* Configure P0 pins */

  PCLKSEL1 = 0x00000004;	// GPIO peripheral clock is CCLK/1

  PINSEL0 = 0x00000000;		// Every P0 pin is a GPIO
  PINSEL1 = 0x00000000;

  FIO0MASK = 0x00000000;	// Every P0 pin is unmasked
  FIO0DIR = 0xFFFFFFFF;		// Every P0 pin is an output

  for (i = 0;; i++)
    FIO0PIN = i;
}

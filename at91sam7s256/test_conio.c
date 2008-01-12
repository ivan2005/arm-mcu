/* Simple serial console I/O test program */

// $Id: test_conio.c,v 1.2 2008-01-12 21:31:24 cvs Exp $

#include <conio.h>
#include <cpu.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
  char buf[32];

  cpu_init(DEFAULT_CPU_FREQ);
  conio_init((unsigned long int) AT91C_BASE_US0, 19200);

  puts("\033[H\033[2JAT91SAM7S256 Console I/O Test\n");

  for (;;)
  {
    printf("Enter a string: ");
    fgets(buf, sizeof(buf), stdin);
    if (strlen(buf)) if (buf[strlen(buf)-1] == '\n') buf[strlen(buf)-1] = 0;
    printf("You entered %ld bytes, '%s'\n", strlen(buf), buf);
  }
}

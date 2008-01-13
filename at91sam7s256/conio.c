/******************************************************************************/
/*                                                                            */
/*   Simple serial console I/O routines for the Atmel AT91SAM7S256 ARM MCU    */
/*                                                                            */
/******************************************************************************/

// $Id: conio.c,v 1.1 2008-01-13 21:00:50 cvs Exp $

#include <cpu.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>

/* Define relocatable UART register definitions */

static unsigned long int UARTBASE = (unsigned long int) AT91C_BASE_US0;

#define US_CR	(*(volatile unsigned long *)(UARTBASE + 0x00))
#define US_MR	(*(volatile unsigned long *)(UARTBASE + 0x04))
#define US_CSR	(*(volatile unsigned long *)(UARTBASE + 0x14))
#define US_RHR	(*(volatile unsigned long *)(UARTBASE + 0x18))
#define US_THR	(*(volatile unsigned long *)(UARTBASE + 0x1C))
#define US_BRGR	(*(volatile unsigned long *)(UARTBASE + 0x20))

/* Initialize serial console */

void conio_init(unsigned long int uartaddr, unsigned long int baudrate)
{
  UARTBASE = uartaddr;

// Configure peripheral clock and I/O pins for UART

  switch (UARTBASE)
  {
    case 0xFFFC0000 :
      *AT91C_PMC_PCER = 0x00000040;
      *AT91C_PIOA_ASR = 0x00000060;
      *AT91C_PIOA_PDR = 0x00000060;
      break;

    case 0xFFFC4000 :
      *AT91C_PMC_PCER = 0x00000080;
      *AT91C_PIOA_ASR = 0x00600000;
      *AT91C_PIOA_PDR = 0x00600000;
      break;
  }

// Configure UART

  US_CR = 0x00000050;
  US_MR = 0x000008C0;
  US_BRGR = CPUFREQ/16/baudrate;
}

/* Send 1 character */

void putch(unsigned char c)
{
  if (c == '\n') putch('\r');

  while ((US_CSR & AT91C_US_TXRDY) == 0);
  US_THR = c;
}

/* Receive 1 character */

unsigned char getch(void)
{
  while ((US_CSR & AT91C_US_RXRDY) == 0);
  return US_RHR;
}

/* Return 1 if key pressed */

unsigned char keypressed(void)
{
  return (US_CSR & AT91C_US_RXRDY);
}

/* Send a string */

void cputs(char *s)
{
  while (*s)
    putch(*s++);
}

/* Override fgets() with a version that does line editing */

char *fgets(char *s, int bufsize, FILE *f)
{
  char *p;
  int c;

  memset(s, 0, bufsize);

  p = s;

  for (p = s; p < s + bufsize-1;)
  {
    c = getchar();
    switch (c)
    {
      case '\r' :
      case '\n' :
        putchar('\r');
        putchar('\n');
        *p = '\n';
        return s;

      case '\b' :
        if (p > s)
        {
          *p-- = 0;
          putchar('\b');
          putchar(' ');
          putchar('\b');
        }
        break;

      default :
        putchar(c);
        *p++ = c;
        break;
    }
  }

  return s;
}

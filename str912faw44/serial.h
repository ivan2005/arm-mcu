/******************************************************************************/
/*                                                                            */
/*        Simple serial port I/O routines for the STR912FAW44 ARM MCU         */
/*                                                                            */
/******************************************************************************/

// $Id: serial.h,v 1.1 2008-08-14 20:08:18 cvs Exp $

#define MAX_SERIAL_PORTS	3

/* Initialize serial port */

int serial_init(unsigned port, unsigned long int baudrate);

/* Register serial port for standard I/O */

int serial_stdio(unsigned port, unsigned long int baudrate);

/* Return TRUE if transmitter is ready to accept data */

int serial_txready(unsigned port);

/* Write data to the serial port */

int serial_write(unsigned port, char *buf, size_t count);

/* Return TRUE if receive data is available */

int serial_rxready(unsigned port);

/* Read data from the serial port */

int serial_read(unsigned port, char *buf, size_t count);
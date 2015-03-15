/**
 * Dada Kernel
 *
 * Wiktor Lukasik (wiktor@lukasik.org)
 */
#ifndef io_h
#define io_h

#include "types.h"

#define IO_PORT                             0x3f8
#define IO_SERIAL_COM1_BASE                 0x3F8
#define IO_SERIAL_DATA_PORT(base)           (base)
#define IO_SERIAL_FIFO_COMMAND_PORT(base)   (base + 2)
#define IO_SERIAL_LINE_COMMAND_PORT(base)   (base + 3)
#define IO_SERIAL_MODEM_COMMAND_PORT(base)  (base + 4)
#define IO_SERIAL_LINE_STATUS_PORT(base)    (base + 5)
#define IO_SERIAL_LINE_ENABLE_DLAB          0x80

/* Writes data to port */
void outportb(unsigned short port, unsigned char data);
void outportl(unsigned short port, unsigned int data);

/* Reads data from a port */
unsigned char inportb(unsigned short port);
unsigned int inportl(unsigned short port);
uint16_t inportws(unsigned short port);

/* DOCUMENT ALL OF THIS */
void io_serial_configure_baud_rate(unsigned short com, unsigned short divisor);
void io_serial_configure_line(unsigned short com);
int io_serial_is_transmit_fifo_empty(unsigned int com);
void init_serial();
int io_is_transmit_empty();

#endif

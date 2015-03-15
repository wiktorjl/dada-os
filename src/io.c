/**
 * Dada Kernel
 *
 * Wiktor Lukasik (wiktor@lukasik.org)
 */

#include "io.h"

void io_serial_configure_baud_rate(unsigned short com, unsigned short divisor)
{
    outportb(IO_SERIAL_LINE_COMMAND_PORT(com), IO_SERIAL_LINE_ENABLE_DLAB);
    outportb(IO_SERIAL_DATA_PORT(com), (divisor >> 8) & 0x00FF);
    outportb(IO_SERIAL_DATA_PORT(com), divisor & 0x00FF);
}

void io_serial_configure_line(unsigned short com)
{
    /* Bit: | 7 | 6 | 5 4 3 | 2 | 1 0 |
    * Content: | d | b | prty | s | dl |
    * Value: | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03
    */
    outportb(IO_SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

int io_serial_is_transmit_fifo_empty(unsigned int com)
{
    /* 0x20 = 0001 0000 */
    return inportb(IO_SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}


void init_serial() {
    outportb(IO_PORT + 1, 0x00);    // Disable all interrupts
    outportb(IO_PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outportb(IO_PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outportb(IO_PORT + 1, 0x00);    //                  (hi byte)
    outportb(IO_PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
    outportb(IO_PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outportb(IO_PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

int io_is_transmit_empty() {
   return inportb(IO_PORT + 5) & 0x20;
}

void outportb(unsigned short port, unsigned char data) {
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (data));
}

void outportl(unsigned short port, unsigned int data) {
    asm volatile ("outl %1, %0" : : "d" (port), "a" (data));
}

unsigned char inportb(unsigned short port) {
    unsigned char rv;
    asm volatile ("inb %1, %0" : "=a" (rv) : "dN" (port));
    return rv;
}

unsigned int inportl(unsigned short port) {
    uint32_t rv;
    asm volatile ("inl %1, %0" : "=a" (rv) : "d" (port));
    return rv;
}

uint16_t inportws(unsigned short port) {
    uint16_t rv;
    asm volatile ("inw %1, %0" : "=a" (rv) : "d" (port));
    return rv;
}


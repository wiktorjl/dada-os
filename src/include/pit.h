/**
 * Dada Kernel
 *
 * Wiktor Lukasik (wiktor@lukasik.org)
 */

#ifndef pit_h
#define pit_h

#include "types.h"
#include "io.h"

#define PIT_FREQ                                1193180
#define HZ                                      1000

#define PIT_COUNTERTYPE_BINARY                  0x0
#define PIT_COUNTERTYPE_BCD                     0x1

#define PIT_OPERATING_MODE_INTTERMCOUNT         0x0
#define PIT_OPERATING_MODE_ONESHOT              0x2
#define PIT_OPERATING_MODE_RATEGENERATOR        0x4
#define PIT_OPERATING_MODE_SQWAVEGENERATOR      0x6
#define PIT_OPERATING_MODE_SOFTWAREREIGGERED    0x8
#define PIT_OPERATING_MODE_HARDWARETRIGGERED    0xA

#define PIT_READLOADMODE_LATCH                  0x0
#define PIT_READLOADMODE_LSB                    0x10
#define PIT_READLOADMODE_MSB                    0x20
#define PIT_READLOADMODE_LSBMSB                 0x30

#define PIT_SELCOUNTER_0                        0x0
#define PIT_SELCOUNTER_1                        0x40
#define PIT_SELCOUNTER_2                        0x80

#define PIT_REG_COUNTER_0                       0x40
#define PIT_REG_COUNTER_1                       0x41
#define PIT_REG_COUNTER_2                       0x42
#define PIT_REG_COMMAND                         0x43

#define PIT_DEFAULT_COUNTER                     PIT_REG_COUNTER_0

#define PIT_SEND_COMMAND(COUNTER, COMMAND)      outportb(PIT_REG_COMMAND, COMMAND);
#define PIT_SEND_DATA(COUNTER, DATA)            outportb(COUNTER, DATA);

#define PIT_INIT_COUNTER0                       PIT_SEND_COMMAND( \
                                                    PIT_SELCOUNTER_0, \
                                                    PIT_OPERATING_MODE_RATEGENERATOR | PIT_READLOADMODE_LSBMSB | PIT_SELCOUNTER_0)

#define PIT_INIT_FREQ(LOWER, UPPER)             PIT_SEND_DATA( \
                                                    PIT_DEFAULT_COUNTER, LOWER) \
                                                PIT_SEND_DATA( \
                                                    PIT_DEFAULT_COUNTER, UPPER)

extern uint32_t timer_ticks;

void pit_init();

#endif

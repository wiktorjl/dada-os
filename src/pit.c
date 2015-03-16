/**
 * Dada Kernel
 *
 * Wiktor Lukasik (wiktor@lukasik.org)
 */

#include "console.h"
#include "idt.h"
#include "io.h"
#include "pit.h"
#include "types.h"

extern void isrtimer();

uint32_t timer_ticks = 0;
uint32_t timer_count = 0;

void pit_irq_handler() {
    timer_ticks++;

    if(--timer_count == 0) {
        // one second?
        timer_count = HZ;
    }

    irq_ack(IRQ_ISR_TIMER);
}

void pit_init() {
    uint32_t freq = (uint32_t) PIT_FREQ;
    uint16_t hz = (uint16_t) HZ;
    uint16_t rate = (uint16_t) (freq / hz);
    uint8_t lower_freq = rate & 0xff;
    uint8_t upper_freq = (rate >> 8) & 0xff;

    // Set up timer 0
    PIT_INIT_COUNTER0;

    // Set frequency
    PIT_INIT_FREQ(lower_freq, upper_freq);

    timer_count = HZ;

    // Set IRQ handler
    idt_setup_gate(IRQ_ISR_TIMER, isrtimer);
}

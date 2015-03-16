/**
 * Dada Kernel
 *
 * Wiktor Lukasik (wiktor@lukasik.org)
 */

#include "pic.h"
#include "io.h"

void pic_init() {
    PIC_INIT
    outportb(0x21,0xfc);
    outportb(0xa1,0xff);
}

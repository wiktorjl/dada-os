/**
 * Dada Kernel
 * October 2014
 *
 * This file is the entry point for the kernel. Called from loader.s.
 * Responsible for initialization of stuff, like interrupt handling, memory and such.
 *
 * Wiktor Lukasik (wiktor@lukasik.org)
 */

/* This is kernel. */
#include "console.h"
#include "types.h"
#include "string.h"
#include "idt.h"
#include "gdt.h"
#include "irq.h"
#include "sys.h"
#include "physmem.h"
#include "vmm.h"
#include "io.h"
#include "kheap.h"
//#include "pata.h"

extern unsigned int * kernel_begin;
extern unsigned int * kernel_end;

extern int * _stack_bottom;
extern int * _stack_top;
extern char cpu_name[15];

void kmain(int * s)
{   
    disable_interrupts();

    /* Compute total kernel size based on symbols from the linker. */
    unsigned int kernel_size = &kernel_end - &kernel_begin;

    /* Point multi boot info structure to whatever is at EBX (set up by loader) */
    struct mbootinfo * bi = (struct mbootinfo *) s;

    console_cls();
    cpuid();
    printk("Dada v0.0.1 | CPU:  %s | %u | 0x%x - 0x%x\n", cpu_name, kernel_size, &kernel_begin, &kernel_end);
    
    /* Initialize physical memory manager based on memory map passed in by multiboot boot loader */
    pmm_initialize_from_mboot(bi);

    printk("Stack: 0x%x - 0x%x (%d bytes)\n", &_stack_bottom, &_stack_top, &_stack_top - &_stack_bottom);
    printk("Lower memory: %u KB, higher memory: %u KB\n", bi->memlow , bi->memhigh );
    
    /* Setup gdt, idt, remap pic, setup irq handlers */
    setup_gdt();
    setup_idt();
    remap_pic();
    setup_irq_gates();
    flash_idt();

    /* Setup page tables and enable paging */
    kheap_init();
    vmm_init();

    /* Enable keyboard */
    init_kbd();

    /* At this point we are ready for interrupts, so enable them */
    enable_interrupts();

    printk("Initialization finished.\n");    
}



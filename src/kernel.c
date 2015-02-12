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
#include "experiment/current/experiment.h"

extern unsigned int * kernel_begin;
extern unsigned int * kernel_end;

extern int * _stack_bottom;
extern int * _stack_top;
extern char cpu_name[15];

extern int _check_paging();
extern void _load_page_directory(unsigned int * page_directory);
extern void _enable_paging();

unsigned int page_directory[1024] __attribute__((aligned(4096)));
unsigned int first_page_table[1024] __attribute__((aligned(4096)));
 
void kmain(int * s)
{   
    unsigned int kernel_size = &kernel_end - &kernel_begin;

    /* Point multi boot info structure to whatever is at EBX (set up by loader) */
    struct mbootinfo * bi = (struct mbootinfo *) s;

    console_cls();
    cpuid();
    printk("Dada v0.0.1 | CPU:  %s | %u | 0x%x - 0x%x\n", cpu_name, kernel_size, &kernel_begin, &kernel_end);
    pmm_initialize_from_mboot(bi);
    printk("Stack: 0x%x - 0x%x (%d bytes)\n", &_stack_bottom, &_stack_top, &_stack_top - &_stack_bottom);
    printk("BIOS reported lower memory: %u KB\n", bi->memlow );
    printk("BIOS reported higher memory: %u KB\n", bi->memhigh );
    setup_gdt();
    setup_idt();
    remap_pic();
    setup_irq_gates();
    flash_idt();
    pmm_print_summary();
    enable_interrupts();
    vmm_init();
}



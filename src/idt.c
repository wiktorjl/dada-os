/**
 * Dada Kernel
 *
 * Wiktor Lukasik (wiktor@lukasik.org)
 */

#include "io.h"
#include "types.h"
#include "idt.h"
#include "string.h"
#include "console.h"
#include "sys.h"
#include "gdt.h"

idt_entry idt_entries[IDT_NUM_ENTRIES];

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

void enable_interrupts() {
    asm volatile("sti");
}

void disable_interrupts() {
    asm volatile("cli");
}

void idt_flush() {
    idtr idtdesc;

    idtdesc.idt_limit = sizeof(struct idt_entry) * IDT_NUM_ENTRIES;
    idtdesc.idt_addr = (uint32_t) &idt_entries;

    asm volatile("lidt %0" : : "m" (idtdesc) : "memory");
}

void idt_setup_gate(int gate, void (*func_base)()) {
    unsigned int base = (unsigned int) func_base;
    idt_entries[gate].base_address_0_15 = base & 0xFFFF;
    /* First entry in GDT, superuser code segment. Remember GDT entries are 8 bytes and first is NULL */
    idt_entries[gate].segment_selector= GDT_SEGMENT_SUPER_CODE * 8 ;
    idt_entries[gate].gap = 0;
    idt_entries[gate].zero = 0;
    idt_entries[gate].flags = IDT_DESCRIPTOR_SEGMENT_PRESENT | IDT_PRIV_KERNEL | IDT_GATE_SIZE_32 | IDT_DESCRIPTOR_TYPE_INTERRUPT; //0x8E;
    idt_entries[gate].base_address_16_31 = (base >> 16) & 0xFFFF;;

}

void idt_isr_default_handler(struct cpu_registers args) { // registers_t r) {
    printk(">>> INTERRUPT: INT# 0x%x ERR# 0x%x\n", args.isr, args.err);
    print_registers(args);
    
    if(args.isr == 0xe) {
        panic("PAGE FAULT!");
    }

    if(args.isr == 0) {
        printk("TIC\n");
        outportb(0x20, 0x20);
    }

}

void idt_init() {
    memset(&idt_entries, 0, sizeof(idt_entry) * IDT_NUM_ENTRIES);
    
    idt_setup_gate(0, &isr0);
    idt_setup_gate(1, &isr1);
    idt_setup_gate(2, &isr2);
    idt_setup_gate(3, &isr3);
    idt_setup_gate(4, &isr4);
    idt_setup_gate(5, &isr5);
    idt_setup_gate(6, &isr6);
    idt_setup_gate(7, &isr7);
    idt_setup_gate(8, &isr8);
    idt_setup_gate(9, &isr9);
    idt_setup_gate(10, &isr10);
    idt_setup_gate(11, &isr11);
    idt_setup_gate(12, &isr12);
    idt_setup_gate(13, &isr13);
    idt_setup_gate(14, &isr14);
    idt_setup_gate(15, &isr15);
    idt_setup_gate(16, &isr16);
    idt_setup_gate(17, &isr17);
    idt_setup_gate(18, &isr18);
    idt_setup_gate(19, &isr19);
    idt_setup_gate(20, &isr20);
    idt_setup_gate(21, &isr21);
    idt_setup_gate(22, &isr22);
    idt_setup_gate(23, &isr23);
    idt_setup_gate(24, &isr24);
    idt_setup_gate(25, &isr25);
    idt_setup_gate(26, &isr26);
    idt_setup_gate(27, &isr27);
    idt_setup_gate(28, &isr28);
    idt_setup_gate(29, &isr29);
    idt_setup_gate(30, &isr30);
    idt_setup_gate(31, &isr31);
}

void idt_init_irq() {
    idt_setup_gate(32, irq0);
    idt_setup_gate(33, irq1);
    idt_setup_gate(34, irq2);
    idt_setup_gate(35, irq3);
    idt_setup_gate(36, irq4);
    idt_setup_gate(37, irq5);
    idt_setup_gate(38, irq6);
    idt_setup_gate(39, irq7);
    idt_setup_gate(40, irq8);
    idt_setup_gate(41, irq9);
    idt_setup_gate(42, irq10);
    idt_setup_gate(43, irq11);
    idt_setup_gate(44, irq12);
    idt_setup_gate(45, irq13);
    idt_setup_gate(46, irq14);
    idt_setup_gate(47, irq15);
}

void irq_ack(unsigned int irq_no) {
    if (irq_no >= 12) {
        outportb(0xA0, 0x20);
    }
    outportb(0x20, 0x20);
}

void irq_default_handler(struct irq_registers regs) {

    // Timer
    if(regs.irq == 0x20) {
        outportb(0x20, 0x20);
        outportb(0xa0,0x20);
        return;
    }

    // Keyboard
    if(regs.irq == 0x21) {
        printk("KEY\n");
        inportb(0x60);
    }

    //irq_ack(regs.irq);
    //outportb(0x20, 0x20);
    //outportb(0xa0,0x20);
    //maskIRQ(0x0);
    return;

    //irq_ack(irqno);
}


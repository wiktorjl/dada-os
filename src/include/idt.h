/**
 * Dada Kernel
 *
 * Wiktor Lukasik (wiktor@lukasik.org)
 */
#ifndef idt_h
#define idt_h

#include "sys.h"
#include "types.h"

#define IDT_NUM_ENTRIES 256

#define IDT_DESCRIPTOR_SEGMENT_PRESENT 0x80
#define IDT_DESCRIPTOR_SEGMENT_NOT_PRESENT 0x0

#define IDT_PRIV_KERNEL 0x0
#define IDT_PRIV_USER 0x60

#define IDT_GATE_SIZE_16 0x0
#define IDT_GATE_SIZE_32 0x8

#define IDT_DESCRIPTOR_TYPE_INTERRUPT 0x6
#define IDT_DESCRIPTOR_TYPE_TRAP 0x7

/* This represents contents of IDTR register. */
typedef struct idtr {
    uint16_t idt_limit;
    uint32_t idt_addr;
} __attribute__((__packed__)) idtr;

/* Entry in the IDT table */
typedef struct idt_entry {
   uint16_t base_address_0_15;
   uint16_t segment_selector;
   uint8_t gap:5;
   uint8_t zero:3;
   uint8_t flags;
   uint16_t base_address_16_31;
} __attribute__((__packed__)) idt_entry;

/* Helper structure to hold register values. This should probably be elsewhere. */
typedef struct registers
{
    unsigned int ds;
    unsigned edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int int_no, err_code;
    unsigned int eip, cs, eflags, useresp, ss;
} registers_t;

/* Register values passed during irq handling. */
struct irq_registers {
    unsigned int gs;
    unsigned int fs;
    unsigned int es;
    unsigned int ds;
    unsigned int edi;
    unsigned int esi;
    unsigned int ebp;
    unsigned int esp;
    unsigned int ebx;
    unsigned int edx;
    unsigned int ecx;
    unsigned int eax;
    unsigned int irq;
    unsigned int err;
};

extern idt_entry idt_entries[IDT_NUM_ENTRIES];

void enable_interrupts();
void disable_interrupts();
void idt_clear();
void idt_flush();
void idt_init();
void idt_setup_gate(int gate, void (*func_base)());
void idt_isr_default_handler(struct cpu_registers args);

/* IRQ related, requires PIC to be initialized */
void idt_init_irq();
void irq_ack(unsigned int irq_no);
void irq_default_handler(struct irq_registers regs);
void irq_mask(unsigned char irq);
void irq_unmask(unsigned char irq);

#endif

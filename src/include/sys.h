/**
 * Dada Kernel
 *
 * Wiktor Lukasik (wiktor@lukasik.org)
 */
#ifndef sys_h
#define sys_h
#include "types.h"

/* Contains name of the CPU as reported by cpuid. Populated by void cpuid(); */
char cpu_name[15];

typedef struct memoryMap
{
  unsigned long Size;
  unsigned long BaseAddrLow;
  unsigned long BaseAddrHigh;
  unsigned long LengthLow;
  unsigned long LengthHigh;
  unsigned long Type;
} __attribute__ ((packed)) memoryMap;

/* Flags and memory range passed in by multiboot compatibile bootloader */
struct mbootinfo {
    unsigned long flags;
    unsigned long memlow;
    unsigned long memhigh;
    unsigned char bootDevice[4];
    char *commandLine;
    unsigned long modulesCount;
    unsigned int *modules;
    unsigned long num;
    unsigned long size;
    unsigned long addr;
    unsigned long reserved;
    unsigned long mmap_length;
    memoryMap *mmap_addr;
    unsigned long drives_length;
    unsigned long drives_addr;
    unsigned long config_table;
    unsigned long bootloader_name;
};

struct cpu_registers {
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
    unsigned int isr;
    unsigned int err;
};

/* Executes cpuid instruction and populates cpu_name with name of the processor. */
void cpuid();

/* Keep calm and panic. */
void panic(char * msg);

/* Do nothing forever. */
void halt_and_catch_fire();

#endif


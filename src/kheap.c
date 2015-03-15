#include "kheap.h"
#include "physmem.h"
#include "console.h"

unsigned int * kheap;
unsigned int * kheap_top;
extern unsigned int * kernel_end;

void kheap_init() {
    kheap_top = (unsigned int *) pmm_get_memory_size();
    kheap = kheap_top - KHEAP_SIZE;
    pmm_remove_memory_region((unsigned int)kheap, (unsigned int)kheap_top);
    printk("Kernel heap: 0x%x-0x%x\n", kheap, kheap_top);
}

void * kmalloc(unsigned int n) {
    if(n == 0) {
        return 0;
    } else {
        if(kheap + n > kheap_top) {
            panic("Uh, oh, no more heap!");
        }

        unsigned int * retval = kheap;
        kheap += n;
        return retval;
    }
}

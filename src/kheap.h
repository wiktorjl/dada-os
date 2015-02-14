#ifndef kheap_h
#define kheap_h

#define KHEAP_SIZE 0x100000 // 1mb of kernel heap

void kheap_init();

void * kmalloc(unsigned int n);
#endif

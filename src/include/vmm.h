/**
 * Dada Kernel
 *
 * Wiktor Lukasik (wiktor@lukasik.org)
 */

// Based on http://www.brokenthorn.com/Resources/OSDev18.html
#ifndef vmm_h
#define vmm_h

#define VMM_NUM_PD_ENTRIES 1024
#define VMM_NUM_PT_ENTRIES 1024

#define VMM_PAGE_SIZE 4096

#define VMM_MASK_PT_PRESENT     1  // 0000 0000 0000 0001
#define VMM_MASK_PT_RW          2  // 0000 0000 0000 0010
#define VMM_MASK_PT_MODE        4  // 0000 0000 0000 0100
#define VMM_MASK_PT_ACCESSED    32 // 0000 0000 0010 0000
#define VMM_MASK_PT_DIRTY       64 // 0000 0000 0100 0000
#define VMM_MASK_PT_FRAME       4294963200 // 1111 1111 1111 1111 1111 0000 0000 0000

#define VMM_MASK_PD_PRESENT     1  // 0000 0000 0000 0001
#define VMM_MASK_PD_RW          2  // 0000 0000 0000 0010

#define VMM_CLEAR_PAGE(p) memset(p, 0 , 4096 * sizeof(vmm_entry))

/* Represents both page directory entry and page table entry */
typedef unsigned int vmm_entry;
typedef vmm_entry vmm_pdt_entry[VMM_NUM_PT_ENTRIES];
typedef vmm_pdt_entry vmm_pdt[VMM_NUM_PD_ENTRIES];

/* Page fault handler */
void page_fault_handler();

/* Set attribute on either page table entry or page directory entry */
void vmm_set_entry_attribute(vmm_entry * entry, unsigned int attr);
/* Set address on either page table entry or page directory entry */
void vmm_set_entry_address(vmm_entry * entry, unsigned int addr);

/* Initialize virtual memory */
void vmm_init();

#endif

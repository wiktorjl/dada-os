/**
 * Dada Kernel
 *
 * Wiktor Lukasik (wiktor@lukasik.org)
 */

#include "physmem.h"
#include "vmm.h"
#include "string.h"
#include "console.h"
#include "math.h"

/* ASM function to enable paging */
extern void _enable_paging();
/* Get contents of CR2 register, useful during page fault */
extern unsigned char * _get_cr2();
extern unsigned int * vmmstart;

/* Points to block containing page directory */
unsigned int * vmm_dir;

void page_fault_handler() {
    panick("Page fault occured at virtual address 0x%x!\n", _get_cr2());
}

void vmm_set_entry_attribute(vmm_entry * entry, unsigned int attr) {
    *entry |= attr;
}

void vmm_set_entry_address(vmm_entry * entry, unsigned int addr) {
    *entry = (*entry | ((*entry & VMM_MASK_PT_FRAME) | addr));
}

vmm_entry * vmm_create_page_directory() {
    int freeblk = pmm_next_free_at(&vmmstart);

    if(freeblk == -1) {
        panic("Could not find free block for page directory!");
    }

    int blk = pmm_reserve_block(freeblk);

    if(blk == -1) {
        panic("No more free blocks for page directory!");
    }
    
    vmm_entry * dentry = pmm_block_to_location(freeblk);
    logk("VMM INIT: Page directory at 0x%x\n", dentry);
    VMM_CLEAR_PAGE(dentry);

    vmm_set_entry_attribute(dentry, VMM_MASK_PD_PRESENT);
    vmm_set_entry_attribute(dentry, VMM_MASK_PD_RW);

    return dentry;
}

vmm_entry * vmm_create_page_table() {
    int freeblk = pmm_next_free_at(&vmmstart);

    if(freeblk == -1) {
        panic("Could not find free block for page table!");
    }

    int blk = pmm_reserve_block(freeblk);

    if(blk == -1) {
        panic("No more free blocks for page table!");
    }

    if(blk == -1) {
        panic("No more free blocks!");
    }
    
    vmm_entry * pentry = pmm_block_to_location(freeblk);
    logk("VMM INIT: Page tables at 0x%x\n", pentry);
    VMM_CLEAR_PAGE(pentry);

    vmm_set_entry_attribute(pentry, VMM_MASK_PT_PRESENT);

    return pentry;
}

void vmm_init() {
    unsigned int total_mem = pmm_get_memory_size();
    int total_pdirs = 0;
    int total_ptables = 0;
    
    total_ptables = CEIL(total_mem, 4096);
    total_pdirs = CEIL(total_ptables, 1024);
    logk("VMM INIT: Total memory size: %u bytes\n", total_mem);
    logk("VMM INIT: Page directories required: %d \n", total_pdirs);
    logk("VMM INIT: Page tables required: %d \n", total_ptables);

    unsigned int phys_addr = 0;
    vmm_entry * dir = vmm_create_page_directory();
    vmm_dir = dir;
   
    logk("VMM INIT: Creating page directories and page tables...\n");
    for(int pd = 0; pd < total_pdirs; pd++) {
        vmm_set_entry_attribute(dir, VMM_MASK_PD_PRESENT);
        vmm_set_entry_attribute(dir, VMM_MASK_PD_RW);
        vmm_entry * ptable = vmm_create_page_table();
        vmm_set_entry_address(dir, ptable);

        vmm_entry * page = ptable;
        int entry_count = 0;
        for(int pt = 0; pt < total_ptables && entry_count < 1024; pt++, entry_count++, total_ptables--) {
            vmm_set_entry_attribute(page, VMM_MASK_PT_PRESENT);
            vmm_set_entry_address(page, (unsigned int) phys_addr);
            phys_addr += 4096;
            page += 1;
        }
        dir += 1;
    }
    logk("VMM INIT: Loading page directory.\n");
    _load_page_directory(vmm_dir);

    setup_idt_gate(0xe, page_fault_handler);

    _enable_paging();

    printk("VMM INIT: Paging enabled.\n");
}



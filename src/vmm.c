#include "physmem.h"
#include "vmm.h"
#include "string.h"
#include "console.h"
#include "math.h"

extern void _enable_paging();
extern unsigned char * _get_cr2();

unsigned int * vmm_dir;

void page_fault_handler() {
    panick("Page fault occured at virtual address 0x%x!\n", _get_cr2());
}

void vmm_set_entry_attribute(vmm_entry * entry, unsigned int attr) {
    *entry |= attr;
}

void vmm_set_entry_address(vmm_entry * entry, unsigned int addr) {
    unsigned int mask = 4294963200;
    unsigned int masked = *entry & mask;
    unsigned int newaddr = masked | addr;
    *entry |= newaddr;
    //*entry = (*entry & VMM_MASK_PT_FRAME) | addr;
}

vmm_entry * vmm_create_page_directory() {
    int blk = pmm_reserve_next_free();

    if(blk == -1) {
        panic("No more free blocks!");
    }
    
    vmm_entry * dentry = pmm_block_to_location(blk);
    VMM_CLEAR_PAGE(dentry);

    vmm_set_entry_attribute(dentry, VMM_MASK_PD_PRESENT);
    vmm_set_entry_attribute(dentry, VMM_MASK_PD_RW);

    return dentry;
}

vmm_entry * vmm_create_page_table() {
    int blk = pmm_reserve_next_free();

    if(blk == -1) {
        panic("No more free blocks!");
    }
    
    vmm_entry * pentry = pmm_block_to_location(blk);
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
    printk("VMM INIT: Total memory size: %d bytes\n", total_mem);
    printk("VMM INIT: Page directories required: %d \n", total_pdirs);
    printk("VMM INIT: Page tables required: %d \n", total_ptables);

    unsigned int phys_addr = 0;
    vmm_entry * dir = vmm_create_page_directory();
    vmm_dir = dir;
   
    printk("VMM INIT: Creating page directories and page tables...\n");
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
    printk("VMM INIT: Loading page directory.\n");
    _load_page_directory(vmm_dir);

    setup_idt_gate(0xe, page_fault_handler);

    printk("VMM INIT: Enabling paging.\n");
    _enable_paging();
    printk("VMM INIT: Welcome in virtual memory!\n");


}

void xvmm_init() {

    // For now, we set up 4 mb to be identity mapped.
    // 4mb = 1 block

    int free_blk = 0; //pmm_reserve_next_free(); 
    vmm_entry * page = 0;

    free_blk = pmm_reserve_next_free(); 
    vmm_entry * dentry = pmm_block_to_location(free_blk);
    printk("VMMINIT: Page directory entry at location: 0x%x/%d\n", dentry, free_blk);
    VMM_CLEAR_PAGE(dentry);
    vmm_set_entry_attribute(dentry, VMM_MASK_PD_PRESENT);
    vmm_set_entry_attribute(dentry, VMM_MASK_PD_RW);

    vmm_entry phys_addr = 0; 
    free_blk = pmm_reserve_next_free(); 
    page = pmm_block_to_location(free_blk);
    VMM_CLEAR_PAGE(page);
    printk("VMMINIT: First page entry: 0x%x/%d\n", page, free_blk);
    for(int i = 0; i < 1024; i++) {
        //printk("VMMINIT: Allocated page %d at location: 0x%x\n", i, page); 
        
        //if(i > 5) break;
        vmm_set_entry_attribute(page, VMM_MASK_PT_PRESENT);
        vmm_set_entry_address(page, (unsigned int) phys_addr);
        printk("\rVMMINIT: Page entry %d: 0x%x", i, page);
        phys_addr += 4096;

        if(i == 0) {
            vmm_set_entry_address(dentry, (unsigned int) page);
            //printk("VMMINIT: Set 0x%x as first entry in directory.\n", page);
        }
        //printk("\tVMMINIT: Page entry: 0x%x\n", *dentry);
        page += 1;
    }
    printk("\nVMMINIT: Allocated pages.\n");
    printk("VMMINIT: Page directory entry: 0x%x\n", *dentry);
    vmm_dir = dentry;
    //((unsigned int *) vmm_dir)[0] = page;

    
    //page |= [M#\
    //vmm_dir[0] = 0;// page;
    // Enable paging
    printk("VMMINIT: Loading page directory.\n");
    _load_page_directory(vmm_dir);

    setup_idt_gate(0xe, page_fault_handler);

    printk("VMMINIT: Enabling paging.\n");
    _enable_paging();
    printk("VMMINIT: Welcome in virtual memory!\n");

    // reference 5 mb
    unsigned int * x = 4194301;
    *x = 7;
}

/**
 * Dada Kernel
 *
 * Wiktor Lukasik (wiktor@lukasik.org)
 */

#include "console.h"
#include "math.h"
#include "string.h"
#include "physmem.h"
#include "sys.h"

extern unsigned int * kernel_begin;
extern unsigned int * kernel_end;

// Total memory in bytes
unsigned int pmm_total_memory_size = 0; 

// Total memory of usable regions in bytes
unsigned int pmm_usable_memory_size = 0;

// Points to the beginning of the memory map (bits to blocks)
unsigned int * pmm_memory_map = 0;

// Memory map in blocks. I.e., if total memory is 3865 bytes,
// we need 36864 / 4096 = 9 blocks to represent it. Since we need 
// one bit per block, we need to see how bytes that is: 9 / 8 = 1 + 1/9 => 2
unsigned int pmm_memory_map_length_in_blocks = 0;

// Total number of blocks
int pmm_total_blocks = 0;

// Number of blocks left for allocation
int pmm_used_blocks = 0;

int pmm_set_block(int bit, int val) {
    if(val) {
        pmm_memory_map[bit / 32] |= (1 << (bit % 32));
    } else {
        pmm_memory_map[bit / 32] &= ~ (1 << (bit % 32));
    }

    return val;
}

int pmm_get_block(int bit) {
    return (pmm_memory_map[bit / 32] &  (1 << (bit % 32))) != 0;
}

void pmm_initialize_from_mboot(struct mbootinfo * bootInfo) {
    unsigned int total_size = 0;
    unsigned int size;
    unsigned int start;
    unsigned int end;
    int i, parts;

    LOGFUNC("pmm_initialize_from_boot");

    // Check how many regions are reported
    parts=((int)bootInfo->mmap_length)/sizeof(memoryMap);
    logk("Number of memory regions reported: %d\n", parts);

    // Grab total memory size based on the maximum end address of a reported memory region
    for (i=0;i<parts;i++) {
        size = bootInfo->mmap_addr[i].LengthLow;
        start = bootInfo->mmap_addr[i].BaseAddrLow;
        end = start + size;
        
        if(end > total_size) total_size = end;

        logk("Memory region: %d - %d, %d\n", start, end, size);
    }

    // Initialize memory map based. Place it at the end of the kernel. Mark all memory as used.
    pmm_initialize((unsigned int) &kernel_end, total_size);

    // For each usable memory region, mark it in the map as availabile.
    printk("Memory map: \n");
    for (i=0;i<parts;i++) {
        start = bootInfo->mmap_addr[i].BaseAddrLow;
        size = bootInfo->mmap_addr[i].LengthLow;
        end = start + size;

        printk("  0x%x - 0x%x (%u bytes)", start, end, size);

        if(1 == bootInfo->mmap_addr[i].Type) {
            printk(" [usable]");
            pmm_add_memory_region(start, end);
        } else {
            printk(" [reserved]");
        }
        printk("\n");
    }

    // Ensure that memory occupied by kernel is marked as reserved.
    pmm_remove_memory_region((unsigned int) &kernel_begin, (unsigned int) &kernel_end);
    // Same foe region occupied by memory map
    pmm_remove_memory_region((unsigned int) &kernel_end, (unsigned int) &kernel_end + total_size / PMM_BLOCK_SIZE / 8);

    printk("Total found usable memory: %u bytes\n", pmm_get_memory_size());
}

void pmm_initialize(unsigned int where, int size) {
    LOGFUNC("pmm_initialize");
    pmm_memory_map = (unsigned int *) where;
    pmm_total_memory_size = size;
    pmm_total_blocks = pmm_total_memory_size / PMM_BLOCK_SIZE;
    pmm_used_blocks = pmm_total_blocks; // We start with all blocks reserved. 

    pmm_memory_map_length_in_blocks = pmm_total_memory_size / PMM_BLOCK_SIZE / 8;
       
    // Mark all blocks to be reserved
    logk("Memset the pmm bitmap\n");
    memset((unsigned int *)where, 0xFFFFFFFF, pmm_total_memory_size / PMM_BLOCK_SIZE / 8);
}

void pmm_set_memory_size(unsigned int size) {
    pmm_total_memory_size = size;
}

unsigned int pmm_get_memory_start() {
    return (unsigned int) pmm_memory_map;
}

unsigned int pmm_get_memory_size() {
    return pmm_total_memory_size;
}

int pmm_location_to_block(unsigned int loc) {
    return loc / PMM_BLOCK_SIZE;
}

int pmm_block_to_location(int blk) {
    return blk * PMM_BLOCK_SIZE;
}

void pmm_modify_memory_region(unsigned int start, unsigned int end, int remove) {

    // Starting block
    int startb = start/PMM_BLOCK_SIZE;
    
    // Number of blocks in memory region
    int endb = CEIL(end, PMM_BLOCK_SIZE);

    int num = endb - startb;

    logk("\n%s memory region %u - %u at block %d-%d with length %d\n", remove?"Removing":"Adding", start, end, startb, endb, num);

    // Mark each block as available
    for(int i = 0; i < num; i++) {
        pmm_set_block(startb++, remove);
        pmm_used_blocks += remove?1:-1;
    }
}


void pmm_add_memory_region(unsigned int start, unsigned int end) {
    pmm_modify_memory_region(start, end, 0);
}

void pmm_remove_memory_region(unsigned int start, unsigned int end) {
    pmm_modify_memory_region(start, end, 1);
}

int pmm_next_free() {
    for(int i = 0; i < pmm_total_blocks; i++) {
        if(pmm_get_block(i) == 0) {
            return i;
        }
    }

    return -1;
}

int pmm_next_free_at(unsigned int loc) {
    int startb = pmm_location_to_block(loc);

    for(int i = startb; i < pmm_total_blocks; i++) {
        if(pmm_get_block(i) == 0) {
            return i;
        }
    }

    return -1;
}

int pmm_reserve_block(int i) {
    int ret = pmm_set_block(i, 1);
    pmm_used_blocks++;

    return ret;
}

int pmm_reserve_next_free() {
    int free = pmm_next_free();

    if(free == -1) {
        return -1;
        //panic("Failed to reserve more memory blocks.");
    }

    pmm_reserve_block(free);

    return free;
}

int pmm_free_block(int i) {
    pmm_used_blocks--;
    return pmm_set_block(i, 0);
}

int pmm_block_status(int block) {
    return pmm_get_block(block);    
}

void pmm_print_summary() {
    printk("PMM internal summary:\n");
    printk("    pmm_mem_size: %d\n    pmm_total_blocks: %d\n    pmm_used_blocks: %d\n", 
        pmm_total_memory_size, pmm_total_blocks, pmm_used_blocks);
}

int pmm_get_total_blocks() {
    return pmm_total_blocks;
}

void pmm_dump_mm() {
    int width = 60;

    for(int i = 0; i < pmm_total_blocks; i++) {
        if(i % width == 0) {
            logk("%s%d | ", i != 0?"\n":"", i / width);
        }
        
        logk("%s", pmm_get_block(i)?"1":"0");
        
    }
    logk("\n");
}


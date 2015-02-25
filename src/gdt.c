/**
 * Dada Kernel
 *
 * Wiktor Lukasik (wiktor@lukasik.org)
 */

#include "console.h"
#include "gdt.h"
#include "types.h"

extern void load_gdt_ptr(void);

struct gdt_segment_descriptor gdt_entries[NUM_GDT_ENTRIES];
struct gdt_pointer gdt_ptr;

void gdt_build_segment_descriptor(int i, uint32_t base_addr, uint32_t size, uint8_t flags, uint8_t granularity) {

	// Set segment size
	gdt_entries[i].limit_0_15 = (size & 0xFFFF);
    gdt_entries[i].limit_16_19 = ((size >> 16) & 0xFFFF);

    // Set segment start address
    gdt_entries[i].base_address_0_15 = (base_addr & 0xFFFF);
    gdt_entries[i].base_address_16_23 = (base_addr >> 16) & 0xFF;
    gdt_entries[i].base_address_24_31 = (base_addr >> 24) & 0xFF;

    // Set flags
    gdt_entries[i].flags1 = flags;
    gdt_entries[i].flags2 = granularity & 0x0F;
}

void gdt_flush() {
    load_gdt_ptr();
}

void gdt_init() {
    uint32_t gdt_base = (unsigned int) &gdt_entries;
    uint16_t gdt_size = (unsigned short) sizeof(struct gdt_segment_descriptor) * NUM_GDT_ENTRIES;
    
    gdt_ptr.size = gdt_size;
    gdt_ptr.base = gdt_base;

    logk("GDT INIT: Base=0x%x, Size=%d\n", gdt_ptr.base, gdt_ptr.size);

    // Null descriptor (do not use)
    gdt_build_segment_descriptor(0, 0, 0, 0, 0);

    // Code super-user segment
     gdt_build_segment_descriptor(1,
    		GDT_BASE_SEGMENT_ADDRESS,
			GDT_SEGMENT_SIZE,
			GDT_ATTR_CODE_SUPER_MODE,
			GDT_ATTR_DEFAULT_GRANULARITY);

    // Data super-user segment
    gdt_build_segment_descriptor(2,
    		GDT_BASE_SEGMENT_ADDRESS,
			GDT_SEGMENT_SIZE,
			GDT_ATTR_DATA_SUPER_MODE,
			GDT_ATTR_DEFAULT_GRANULARITY);

    // Code user segment
    gdt_build_segment_descriptor(3,
    		GDT_BASE_SEGMENT_ADDRESS,
			GDT_SEGMENT_SIZE,
			GDT_ATTR_CODE_USER_MODE,
			GDT_ATTR_DEFAULT_GRANULARITY);

    // Data user segment
    gdt_build_segment_descriptor(4,
    		GDT_BASE_SEGMENT_ADDRESS,
			GDT_SEGMENT_SIZE,
			GDT_ATTR_DATA_USER_MODE,
			GDT_ATTR_DEFAULT_GRANULARITY);

    // Load GDT
    gdt_flush();
}

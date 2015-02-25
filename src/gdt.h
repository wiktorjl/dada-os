/**
 * Dada Kernel
 *
 * Wiktor Lukasik (wiktor@lukasik.org)
 */
#ifndef gdt_h
#define gdt_h

#define NUM_GDT_ENTRIES 5

#define GDT_BASE_SEGMENT_ADDRESS                    0x0
#define GDT_SEGMENT_SIZE                            0xFFFFFFFF // 32 bit address

#define GDT_ATTR_SEGMENT_PRESENT                    0x80 // 0b10000000
#define GDT_ATTR_SEGMENT_NOT_PRESENT                0x0 // 0b00000000

#define GDT_ATTR_PRIVLVL_SUPER                      0x0 // 0b00000000, ring 0
#define GDT_ATTR_PRIVLVL_USER                       0x60 // 0b01100000, ring 3

#define GDT_ATTR_DESCRIPTOR_TYPE_SYSTEM             0x0
#define GDT_ATTR_DESCRIPTOR_TYPE_CODE               0x10 // 0b00010000

#define GDT_ATTR_SEGMENT_TYPE_CODE                  0x8 // 0b00001000
#define GDT_ATTR_SEGMENT_TYPE_DATA                  0x0 // 0b00000000

#define GDT_ATTR_SEGMENT_TYPE_EXPAND_UP             0x0 // 0b00000000
#define GDT_ATTR_SEGMENT_TYPE_EXPAND_DOWN           0x4 // 0b00000100

#define GDT_ATTR_SEGMENT_TYPE_WRITE_ENABLED         0x2 // 0b00000010
#define GDT_ATTR_SEGMENT_TYPE_WRITE_NOT_ENABLED     0x0 // 0b00000000

#define GDT_ATTR_SEGMENT_TYPE_ACCESSED              0x1 // 0b00000001
#define GDT_ATTR_SEGMENT_TYPE_NOT_ACCESSED          0x0 // 0b00000000

#define GDT_ATTR_SEGMENT_TYPE_CONFORMING            0x4 // 0b00000100
#define GDT_ATTR_SEGMENT_TYPE_NOT_CONFORMING        0x0 // 0b00000000

#define GDT_ATTR_SEGMENT_TYPE_READABLE              0x2 // 0b00000010
#define GDT_ATTR_SEGMENT_TYPE_NOT_READABLE          0x0 // 0b00000000

#define GDT_ATTR_GRANULARITY_1B                     0x0 // 0b0000
#define GDT_ATTR_GRANULARITY_4KB                    0x8 // 0b1000

#define GDT_ATTR_OPSIZE_32                          0x4 // 0b0100
#define GDT_ATTR_OPSIZE_16                          0x0 // 0b0000

#define GDT_ATTR_NOT_64BIT                          0x0
#define GDT_ATTR_IS_64BIT                           0x2 // 0b0010

#define GDT_ATTR_CUSTOM_FLAG                        0x0

#define GDT_ATTR_CODE_SUPER_MODE GDT_ATTR_SEGMENT_PRESENT \
        | GDT_ATTR_PRIVLVL_SUPER \
        | GDT_ATTR_DESCRIPTOR_TYPE_CODE \
        | GDT_ATTR_SEGMENT_TYPE_CODE \
        | GDT_ATTR_SEGMENT_TYPE_NOT_CONFORMING \
        | GDT_ATTR_SEGMENT_TYPE_READABLE \
        | GDT_ATTR_SEGMENT_TYPE_NOT_ACCESSED

#define GDT_ATTR_DATA_SUPER_MODE GDT_ATTR_SEGMENT_PRESENT \
        | GDT_ATTR_PRIVLVL_SUPER \
        | GDT_ATTR_DESCRIPTOR_TYPE_CODE \
        | GDT_ATTR_SEGMENT_TYPE_DATA \
        | GDT_ATTR_SEGMENT_TYPE_EXPAND_UP \
        | GDT_ATTR_SEGMENT_TYPE_WRITE_ENABLED \
        | GDT_ATTR_SEGMENT_TYPE_NOT_ACCESSED

#define GDT_ATTR_CODE_USER_MODE GDT_ATTR_SEGMENT_PRESENT \
        | GDT_ATTR_PRIVLVL_USER \
        | GDT_ATTR_DESCRIPTOR_TYPE_CODE \
        | GDT_ATTR_SEGMENT_TYPE_CODE \
        | GDT_ATTR_SEGMENT_TYPE_NOT_CONFORMING \
        | GDT_ATTR_SEGMENT_TYPE_READABLE \
        | GDT_ATTR_SEGMENT_TYPE_NOT_ACCESSED

#define GDT_ATTR_DATA_USER_MODE GDT_ATTR_SEGMENT_PRESENT \
        | GDT_ATTR_PRIVLVL_USER \
        | GDT_ATTR_DESCRIPTOR_TYPE_CODE \
        | GDT_ATTR_SEGMENT_TYPE_DATA \
        | GDT_ATTR_SEGMENT_TYPE_EXPAND_UP \
        | GDT_ATTR_SEGMENT_TYPE_WRITE_ENABLED \
        | GDT_ATTR_SEGMENT_TYPE_NOT_ACCESSED

#define GDT_ATTR_DEFAULT_GRANULARITY GDT_ATTR_GRANULARITY_4KB \
        | GDT_ATTR_OPSIZE_32 \
        | GDT_ATTR_NOT_64BIT \
        | GDT_ATTR_CUSTOM_FLAG

struct gdt_pointer {
    unsigned short size;
    unsigned int base;
}__attribute__((__packed__));

/* GDT segment descriptor. */

/*
 * Whole segment is 32 bits.
 *
 * Byte 1
 *  0 - 15    Segment limit (0-15)
 * 16 - 31    Base address (0-15)
 *
 * Byte 2
 *  0 -  7    Base address (16-23)
 *  8 - 11    Type
 * 12 - 12    S
 * 13 - 14    DPL
 * 15 - 15    P
 * 16 - 19    Segment limit (16-19)
 * 20 - 20    AVL
 * 21 - 21    L
 * 22 - 22    D/B
 * 23 - 23    G
 * 24 - 31    Base (24-31)
 */
struct gdt_segment_descriptor {
    uint16_t limit_0_15;
    uint16_t base_address_0_15;
    uint8_t base_address_16_23;
    uint8_t flags1 :8;
    uint8_t limit_16_19 :4;
    uint8_t flags2 :4;
    uint8_t base_address_24_31;
}__attribute__((__packed__));

/* Build GDT entry */
void gdt_build_segment_descriptor(int i, uint32_t base_addr, uint32_t size,
        uint8_t flags, uint8_t granularity);

/* Install GDT */
void gdt_init(void);

/* Writes GDT and regreshes segment registers. In ASM. */
void gdt_flush(void);
#endif

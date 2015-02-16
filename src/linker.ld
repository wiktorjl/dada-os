ENTRY(startlower)

SECTIONS
{

    . = 0xC0100000;
    kernel_begin = .;
    
    .text :AT(ADDR(.text) - 0xC0000000)
    {
        *(.text)
        *(.rodata*)
    }

    .data ALIGN(0x1000): AT(ADDR(.data) - 0xC0000000)
    {
        *(.data)
    }

    .bss :AT(ADDR(.bss) - 0xC0000000)
    {
        *(COMMON)
        *(.bss)
        *(.earlystack)
    }

    kernel_end = .;
    vmmstart = .;
}

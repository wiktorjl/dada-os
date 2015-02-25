; Dada Kernel
; October 2014
;
; Assembler helpers for GDT initialization.
;
; Wiktor Lukasik (wiktor@lukasik.org)
 
global load_gdt_ptr

[extern gdt_ptr]
load_gdt_ptr:
    lgdt [gdt_ptr]
; 0x10 is pointer to segment selector. It is an index into GDT, where each entry is 8 bytes, so indexes can be 0x0, 0x8, 0x10, 0x18, 0x20.
; Here we choose third one (remember first is null) - in this case it happens to be ring 0 data segment
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:_load_gdt_ptr ; 0x08 is ring 0 code segment
_load_gdt_ptr:
    ret

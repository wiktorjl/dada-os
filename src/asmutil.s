; Dada Kernel
; February 2015
;
; Assembler helper
;
; Wiktor Lukasik (wiktor@lukasik.org)

global _check_paging
global _load_page_directory
global _enable_paging
global _get_cr2

_get_cr2:
    push ebp
    mov ebp, esp
    mov eax, cr2
    leave
    ret

_check_paging:
    push ebp 
    mov ebp, esp
    mov eax, cr0
    and eax, 0x80000000
    leave
    ret

_load_page_directory:
    push ebp
    mov ebp, esp
    mov eax, [esp+8]
    mov cr3, eax
    leave
    ret

_enable_paging:
    push ebp
    mov ebp, esp
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    leave
    ret


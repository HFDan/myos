%use masm

global kernel_start
extern long_mode_start

section .text
bits 32 ; not in 64 bit long mode yet
kernel_start:
    mov     esp, stack_top
    
    call    multiboot_check
    call    cpuid_check
    call    longmode_check
    
    call    page_table_setup
    call    enable_paging

    lgdt    [gdt64.pointer]
    jmp     gdt64.code_segment:long_mode_start
    
    hlt


multiboot_check:
    cmp     eax, 0x36d76289
    jne     .oof
    ret

    .oof:
    mov     al, "M"
    jmp     err


cpuid_check:
    pushfd
    pop     eax

    mov     ecx, eax ; store a val to compare to
    xor     eax, 1 << 21 ; flip bit 21
    ; push modified eflags
    push    eax
    popfd ; pop them back into eflags
    
    pushfd ; push to see if we managed to flip the cpuid bit
    pop     eax
    
    ; reset eflags
    push    ecx
    popfd

    cmp     eax, ecx
    je      .oof ; no ~bitches~ cpuid?
    ret

    .oof:
        mov     al, "C"
        jmp     err


longmode_check:
    mov     eax, 0x80000000
    cpuid
    cmp     eax, 0x80000001
    jb      .oof

    mov     eax, 0x80000001
    cpuid
    test    edx, 1 << 29 ; check if lm bit is set
    jz      .oof

    ret
    
    .oof:
        ; no ~bitches~ long mode?
        mov     al, "L"
        jmp     err


page_table_setup:
    mov     eax, page_tbl_3
    or      eax, 0b11 ; present, writeable
    mov     [page_tbl_4], eax
    
    mov     eax, page_tbl_2
    or      eax, 0b11 ; present, writeable
    mov     [page_tbl_3], eax

    mov     ecx, 0
    .loop:
        mov eax, 0x200000 ; map in increments of 2MiB. Because it's a *huge page*, you can map in increments of 2MiB
                          ; instead of 4KiB
        mul ecx
        or eax, 0b10000011 ; present, writable, huge page
        mov [page_tbl_2 + ecx * 8], eax

        inc ecx ; increment counter
        cmp ecx, 512 ; checks if the whole table is mapped
        jne .loop ; if not, continue

    ret

enable_paging:
    mov     eax, page_tbl_4
    mov     cr3, eax
    
    ; enable pae
    mov     eax, cr4
    or      eax, 1 << 5
    mov     cr4, eax

    ; enable lm
    mov     ecx, 0xc0000080
    rdmsr
    or      eax, 1 << 8
    wrmsr
    
    ; enable paging
    mov     eax, cr0
    or      eax, 1 << 31
    mov     cr0, eax
    
    ret

err:
    mov     dword ptr [0xb8000], 0x4f524f45
    mov     dword ptr [0xb8004], 0x4f3a4f52
    mov     dword ptr [0xb8008], 0x4f204f20
    mov     byte ptr [0xb800a], al 
    hlt

section .bss

align 4096

page_tbl_4:
    resb 4096
page_tbl_3:
    resb 4096
page_tbl_2:
    resb 4096

stack_bot:
    resb    1024 * 2048 ; 2MiB stack size
stack_top:


section .rodata:
gdt64:
    dq 0
.code_segment: equ $ - gdt64
    dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53) ; in order: executable, descriptor - code/data, present, long mode
.pointer:
    dw $ - gdt64 - 1
    dq gdt64
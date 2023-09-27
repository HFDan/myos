%use masm
bits 64

extern c_exception_handler
global exception_handler


section .text
exception_handler:
    push    rax
    push    rcx
    push    rdx
    push    r8
    push    r9
    push    r10
    push    r11

    lea     rdi, [rsp + 8 * 7]
    mov     rsi, [rsp + 8 * 8]
    cld     ; SystemV ABI requires DF flag to be 0
    call    c_exception_handler

    pop     r11
    pop     r10
    pop     r9
    pop     r8
    pop     rdx
    pop     rcx
    pop     rax
    iretq

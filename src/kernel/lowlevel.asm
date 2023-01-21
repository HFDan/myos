%use masm

extern tty_puts
extern tty_color
global halt

section .text
bits 64

halt:

    mov     dil, 4
    mov     sil, 0
    call    tty_color

    ; print halted text
    lea     rdi, byte ptr [halted_txt]
    call    tty_puts
    hlt

section .rodata:
    halted_txt: db "Halted due to void halt() being called (Intentional)", 0

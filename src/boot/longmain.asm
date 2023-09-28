%use masm

global long_mode_start
extern kmain
extern multiboot_info_data

section .text
bits 64
long_mode_start:
    ; clear all segment registers we don't want
    mov     ax, 0
    mov     ss, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     rdi, [multiboot_info_data]
    call    kmain

    ; prevent executing garbage instructions
    cli
    hlt

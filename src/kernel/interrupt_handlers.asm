%use masm
bits 64

%macro pushallregs 0
    push    rax
    push    rcx
    push    rdx
    push    r8
    push    r9
    push    r10
    push    r11
%endmacro

%macro popallregs 0
    pop     r11
    pop     r10
    pop     r9
    pop     r8
    pop     rdx
    pop     rcx
    pop     rax
%endmacro

%macro irq_handler_error 1
isr_handler_%+%1:
    pushallregs
    mov     rdi, %1
    lea     rsi, [rsp + 8 * 7]
    mov     rdx, [rsp + 8 * 8]
    cld
    call c_exception_handler
    popallregs
    iretq

%endmacro

%macro irq_handler_noerror 1
isr_handler_%+%1:
    pushallregs
    mov     rdi, %1
    lea     rsi, [rsp + 8 * 7]
    cld
    call    c_exception_handler
    popallregs
    iretq
%endmacro


extern c_exception_handler
global exception_stub_table

section .text
irq_handler_noerror 0
irq_handler_noerror 1
irq_handler_noerror 2
irq_handler_noerror 3
irq_handler_noerror 4
irq_handler_noerror 5
irq_handler_noerror 6
irq_handler_noerror 7
irq_handler_error   8
irq_handler_noerror 9
irq_handler_error   10
irq_handler_error   11
irq_handler_error   12
irq_handler_error   13
irq_handler_error   14
irq_handler_noerror 15
irq_handler_noerror 16
irq_handler_error   17
irq_handler_noerror 18
irq_handler_noerror 19
irq_handler_noerror 20
irq_handler_noerror 21
irq_handler_noerror 22
irq_handler_noerror 23
irq_handler_noerror 24
irq_handler_noerror 25
irq_handler_noerror 26
irq_handler_noerror 27
irq_handler_noerror 28
irq_handler_noerror 29
irq_handler_error   30
irq_handler_noerror 31

section .data
exception_stub_table:
%assign i 0
%rep 32
    dq isr_handler_%+i
%assign i i+1
%endrep
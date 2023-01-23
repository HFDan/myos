%use masm

extern kpanic
global halt

section .text
bits 64

;RSP 	ESP 	SP      N/A 	SPL 	Stack Pointer
;RAX 	EAX 	AX 	    AH 	    AL 	    Accumulator
;RBX 	EBX 	BX 	    BH 	    BL 	    Base
;RCX 	ECX 	CX 	    CH 	    CL 	    Counter
;RDX 	EDX 	DX 	    DH      DL 	    Data (commonly extends the A register)
;RSI 	ESI 	SI 	    N/A 	SIL 	Source index for string operations
;RDI 	EDI 	DI 	    N/A 	DIL 	Destination index for string operations
;RBP 	EBP 	BP 	    N/A 	BPL 	Base Pointer (meant for stack frames)
;R8 	R8D 	R8W 	N/A 	R8B 	General purpose
;R9 	R9D 	R9W 	N/A 	R9B 	General purpose
;R10 	R10D 	R10W 	N/A 	R10B 	General purpose
;R11 	R11D 	R11W 	N/A 	R11B 	General purpose
;R12 	R12D 	R12W 	N/A 	R12B 	General purpose
;R13 	R13D 	R13W 	N/A 	R13B 	General purpose
;R14 	R14D 	R14W 	N/A 	R14B 	General purpose
;R15 	R15D 	R15W 	N/A 	R15B 	General purpose 
halt:
    push    rsp
    push    rax
    push    rbx
    push    rcx
    push    rdx
    push    rsi
    push    rdi
    push    rbp
    push    r8
    push    r9
    push    r10
    push    r11
    push    r12
    push    r13
    push    r14
    push    r15

    ; print halted text
    lea     rdi, byte ptr [halted_txt]
    lea     rsi, [rsp]
    call    kpanic

section .rodata:
    halted_txt: db "Halted due to void halt() being called (Intentional)", 0

%use masm

section .multiboot_hdr
hdr_start:

; multiboot2 magic
dd 0xe85250d6

; arch info
dd 0 ; protected mode

; hdr length
dd hdr_end - hdr_start

; checksum
dd 0x100000000 - (0xe85250d6 + 0 + (hdr_end - hdr_start))

; end
dw 0
dw 0
dd 8
hdr_end:
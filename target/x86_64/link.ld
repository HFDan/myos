ENTRY(kernel_start)

SECTIONS
{
    . = 1M;

    .boot :
    {
        KEEP(*(.multiboot_hdr))
    }

    .text : ALIGN(0x1000)
    {
        *(.text)
    }
    
    .idt BLOCK(0x1000) : ALIGN(0x1000)
    {
        _idt = .;
        . = . + 0x1000;
    } 

    .data : ALIGN(0x1000)
    {
        *(.data)
    }

    .rodata : ALIGN(0x1000)
    {
        *(.rodata)
    }

    .bss : ALIGN(0x1000)
    {
        *(COMMON)
        *(.bss)
    }
}
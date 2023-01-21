ENTRY(kernel_start)

SECTIONS
{
    . = 1M;

    .boot :
    {
        KEEP(*(.multiboot_hdr))
    }

    .text :
    {
        *(.text)
    }
}
#include "interrupts.h"
#include "drivers/video/vga.h"
#include "sys/io.h"
#include "pic.h"
#include "printk.h"

static idtr_t idtr;

__attribute__((aligned(0x10)))
static idt_entry_t idt[256];

extern void exception_handler();

void c_exception_handler(uint64_t exception_number, interrupt_frame_t* frame, uint64_t error_code) {
    vga_clear();
    vga_color(VGA_COLOR_RED, VGA_COLOR_BLACK);
    switch (exception_number) {
        case 0:     printk("Divide error!\n"); break;
        case 1:     printk("Debug exception!\n"); break;
        case 2:     printk("NMI Interrupt!\n"); break;
        case 3:     printk("Breakpoint!\n"); break;
        case 4:     printk("Overflow!\n"); break;
        case 5:     printk("Bound range exceeded!\n"); break;
        case 6:     printk("Invalid opcode!\n"); break;
        case 7:     printk("Device not available/No math coprocessor!\n"); break;
        case 8:     printk("Double fault!\n"); break;
        case 9:     printk("Coprocessor Segment Overrun!\n"); break;
        case 10:    printk("Invalid TSS!\n"); break;
        case 11:    printk("Segment not present!\n"); break;
        case 12:    printk("Stack segment fault!\n"); break;
        case 13:    printk("General Protection Fault!\n"); break;
        case 14:    printk("Page Fault!\n"); break;
        case 15:    printk("Reserved interrupt!\n"); break;
        case 16:    printk("Math Fault!\n"); break;
        case 17:    printk("Alignment check!\n"); break;
        case 18:    printk("Machine Check!\n"); break;
        case 19:    printk("SIMD FPE!\n"); break;
        case 20:    printk("Virtualization exception!\n"); break;
        case 21:    printk("Control Protection Exception!\n"); break;
        case 22 ... 31: printk("Reserved interrupt!\n"); break;
    }
    vga_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK);

    printk("rip:        0x%x\n", frame->rip);
    printk("rsp:        0x%x\n", frame->rsp);
    printk("cs:         0x%x\n", frame->cs);
    printk("rflags:     0b%b\n", frame->rflags);
    printk("error code: %i\n", error_code);
    __asm volatile("cli");
    __asm volatile("hlt");
}

void keyboard_handler() {
    PIC_sendEOI(1);
    uint8_t keystroke = inb(0x60);
}


void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* entry = &idt[vector];

    entry->isr_low = (uint64_t)isr & 0xFFFF;
    entry->kernel_cs = 0x08;
    entry->ist = 0;
    entry->attrib = flags;
    entry->isr_middle = ((uint64_t)isr >> 16) & 0xFFFF;
    entry->isr_high = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
    entry->reserved = 0;
}

extern void* exception_stub_table[];

void idt_init() {
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * 35 - 1;

    for (uint8_t vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, exception_stub_table[vector], 0x8f); // 0x8f - trap gate
    }

    PIC_remap(0x20, 0x28);

    __asm volatile ("lidt %0" : : "m"(idtr));
    __asm volatile ("sti");
    printk("Initialized IDT!");
}

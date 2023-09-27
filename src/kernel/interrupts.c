#include "interrupts.h"
#include "drivers/video/vga.h"
#include "sys/io.h"
#include "pic.h"
#include "printk.h"

static idtr_t idtr;

__attribute__((aligned(0x10)))
static idt_entry_t idt[256];

extern void exception_handler();

void c_exception_handler(interrupt_frame_t* frame, uint64_t error_code) {
    vga_clear();
    vga_color(VGA_COLOR_RED, VGA_COLOR_BLACK);
    printk("Hit an interrupt\n");
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

static void (*exception_handlers[32])() = {
    exception_handler,
    exception_handler,
    exception_handler,
    exception_handler,
    exception_handler,
    exception_handler,
    exception_handler,
    exception_handler,
    exception_handler,
    exception_handler,
    exception_handler,
    exception_handler,
    exception_handler,
    exception_handler,
    exception_handler,
    exception_handler,
    exception_handler,
    exception_handler,
    exception_handler,
    exception_handler,
    exception_handler,
    exception_handler,
    exception_handler,
    exception_handler,
    exception_handler,
    exception_handler,
    exception_handler,
    exception_handler,
    exception_handler,
    exception_handler,
    exception_handler,
    exception_handler,
};

static void (*irq_handlers[3])() = {
    exception_handler,
    keyboard_handler,
    keyboard_handler
};

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

extern void* isr_stub_table[];

void idt_init() {
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * 35 - 1;

    for (uint8_t vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, exception_handlers[vector], 0x8f); // 0x8f - trap gate
    }

    for (uint8_t vector = 0; vector < 3; vector++) {
        idt_set_descriptor(vector+31, irq_handlers[vector], 0x8e); // 0x8e interrupt gate
    }

    PIC_remap(0x20, 0x28);

    __asm volatile ("lidt %0" : : "m"(idtr));
    __asm volatile ("sti");
    printk("Initialized IDT!");
}

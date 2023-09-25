#include "idt.h"

static idtr_t idtr;

__attribute__((aligned(0x10)))
static idt_entry_t idt[256];

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

void idt_init() {
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * 1 - 1; // We init an empty IDT for now...

    for (uint8_t vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, 0, 0); // We mark them all as not present
    }

    __asm volatile ("lidt %0" : : "m"(idtr));
    __asm volatile ("sti");
}

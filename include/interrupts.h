#pragma once
#include <stdint.h>

typedef struct {
    uint16_t isr_low;       // Lower 16 bits of ISR addy
    uint16_t kernel_cs;     // GDT segment selector that will be put into CS before calling the ISR
    uint8_t  ist;           // The IST in the TSS that will be loaded into RSP
    uint8_t  attrib;        // Types and attributes
    uint16_t isr_middle;    // The middle 16 bits of ISR addy
    uint32_t isr_high;      // The high 32 bits of ISR addy
    uint32_t reserved;      // Must be 0
} __attribute__((packed)) idt_entry_t;

typedef struct {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) idtr_t;

typedef struct {
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
} __attribute__((packed)) interrupt_frame_t;

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags);
void idt_init(void);
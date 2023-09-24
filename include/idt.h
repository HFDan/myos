#pragma once
#include <stdint.h>

#define IDT_TYPEATTRIB_InterruptGate    0b10001110
#define IDT_TYPEATTRIB_CallGate         0b10001100
#define IDT_TYPEATTRIB_TrapGate         0b10001111

struct idt64_entry {
    uint16_t offset_low; // offset bits 0..15
    uint16_t selector; // code seg selector in gdt or ldt
    uint8_t ist; // bits 0..2, holds interrupt stack table offset, rest of bits should be 0
    uint8_t types_attr; // gate type, dpl, and p fields
    uint16_t offset_mid; // offset bits 16..31
    uint32_t offset_high; // offset bits 32..63
    uint32_t zero; // reserved
};

struct IDTR {
    uint16_t limit;
    uint64_t offset;
} __attribute__((packed));

extern struct idt64_entry _idt[256];

void initIDT();

void idt64_entry_setOffset(struct idt64_entry* this, uint64_t offset);
uint64_t idt64_entry_getOffset(struct idt64_entry* this);
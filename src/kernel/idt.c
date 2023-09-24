#include <stdint.h>
#include "idt.h"

extern uint64_t isr1;

void idt64_entry_setOffset(struct idt64_entry* this, uint64_t offset) {
    this->offset_low = (uint16_t)(offset & 0x000000000000ffff);
    this->offset_mid = (uint16_t)((offset & 0x00000000ffff0000) >> 16);
    this->offset_high = (uint32_t)((offset & 0xffffffff00000000) >> 32);
}

uint64_t idt64_entry_getOffset(struct idt64_entry* this) {
    uint64_t offset = 0;
    offset |= (uint64_t)this->offset_low;
    offset |= (uint64_t)this->offset_mid << 16;
    offset |= (uint64_t)this->offset_high << 32;
    return offset;
}

struct IDTR idtr;
void initIDT() {
    idtr.limit = 0x0fff;
    // TODO
}
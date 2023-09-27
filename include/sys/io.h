#pragma once
#include <stdint.h>

static inline void outb(uint16_t port, uint8_t val) {
    __asm volatile("out %1, %0" : : "a"(val), "Nd"(port) : "memory");
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm volatile("in %0, %1" : "=a"(ret) : "Nd"(port) : "memory");
    return ret;
}

static inline void io_wait() {
    outb(0x80, 0);
}
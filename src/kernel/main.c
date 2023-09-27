#include "printk.h"
#include "interrupts.h"
#include "panic.h"
#include "drivers/video/vga.h"

void kmain() {
    vga_clear();
    idt_init();
    printk("Hello, kernel!");

    panic("Intentional kernel panic");
    for (;;) {
        __asm volatile("hlt");
    }
}
#include "printk.h"
#include "idt.h"
#include "drivers/video/vga.h"

void kmain() {
    idt_init();
    vga_clear();
    printk("Hello, kernel!");
    __asm volatile("hlt");
}
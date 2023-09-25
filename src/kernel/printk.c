#include "printk.h"
#include "drivers/video/vga.h"

void printk(char *str) {
    vga_puts(str);
}
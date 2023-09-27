#include "printk.h"
#include "drivers/video/vga.h"

char* numtobase(uint64_t num, uint32_t base) {
    static char rep[] = "0123456789ABCDEF";
    static char buff[100];
    char* ptr;

    ptr = &buff[49];
    *ptr = '\0';

    do {
        *--ptr = rep[num%base];
        num /= base;
    } while (num != 0);

    return ptr;
}

void printk(char* fmt, ...) {
    char* traverse;

    va_list arg;
    va_start(arg, fmt);

    for (traverse = fmt; *traverse != '\0'; traverse++) {
        while (*traverse != '%') {
            if (*traverse == '\0') return;
            vga_putchar(*traverse);
            traverse++;
        }

        traverse++;

        switch (*traverse) {
            case 'c': {
                int32_t c = va_arg(arg, int32_t);
                vga_putchar(c);
                break;
            }

            case 'i':
            case 'd': {
                int64_t i = va_arg(arg, int64_t);
                if (i < 0) {
                    i = -i;
                    vga_putchar('-');
                }
                vga_puts(numtobase(i, 10));
                break;
            }

            case 'o': {
                int64_t i = va_arg(arg, uint64_t);
                vga_puts(numtobase(i, 8));
                break;
            }

            case 's': {
                char* s = va_arg(arg, char*);
                vga_puts(s);
                break;
            }

            case 'x': {
                uint64_t i = va_arg(arg, uint64_t);
                vga_puts(numtobase(i, 16));
                break;
            }

            case 'b': {
                uint64_t i = va_arg(arg, uint64_t);
                vga_puts(numtobase(i, 2));
                break;
            }
        }
    }

    va_end(arg);
}
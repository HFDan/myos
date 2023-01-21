#include "tty.h"
#include "lowlevel.h"

void kmain() {
    tty_clear();
    tty_color(VGA_COLOR_YELLOW, VGA_COLOR_BLACK);
    tty_puts("Bruh i spent 16 hours just to setup a basic VGA driver :skull:\n");
    halt();
}
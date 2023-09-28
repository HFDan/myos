#include "drivers/video/vga.h"
#include "sys/io.h"

#define VGA_BUFFER_START (struct vgachar*)0xb8000;
static const size_t NUM_COLS = 80;
static const size_t NUM_ROWS = 25;

struct vgachar {
    uint8_t chr;
    uint8_t color;
};

struct vgachar* vgaBuffer = VGA_BUFFER_START;
size_t col = 0;
size_t row = 0;

uint8_t color = VGA_COLOR_WHITE | VGA_COLOR_BLACK << 4;

void vga_enable_cursor(uint8_t start, uint8_t end) {
    outb(0x3d4, 0x0a);
    outb(0x3d5, (inb(0x3d5) & 0xc0) | start);

    outb(0x3d4, 0x0b);
    outb(0x3d5, (inb(0x3d5) & 0xe0) | end);
}

void vga_disable_cursor() {
    outb(0x3d4, 0x0a);
    outb(0x3d5, 0x20);
}

static void __update_cursor(int32_t x, int32_t y) {
    uint16_t pos = y * NUM_COLS + x;

    outb(0x3d4, 0x0f);
    outb(0x3d5, (uint8_t)(pos & 0xff));
    outb(0x3d4, 0x0e);
    outb(0x3d5, (uint8_t)((pos >> 8) & 0xff));
}

static uint16_t __get_cursor_pos() {
    uint16_t pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((uint16_t)inb(0x3D5)) << 8;
    return pos;
}

static void __clear_row(size_t row) {
    struct vgachar empty = {
        .chr =  ' ',
        .color =  color
    };

    for (size_t col = 0; col < NUM_COLS; col++) {
        vgaBuffer[col + NUM_COLS*row] = empty;
    }
}

static void __print_newline() {
    col = 0;

    if (row < NUM_ROWS - 1) {
        row++;
        return;
    }

    for (size_t row = 1; row < NUM_ROWS; row++) {
        for (size_t col = 0; col < NUM_COLS; col++) {
            struct vgachar chr = vgaBuffer[col + NUM_COLS*row];
            vgaBuffer[col + NUM_COLS* (row-1)] = chr;
        }
    }
    __clear_row(NUM_COLS - 1);
}

void vga_clear() {
    for (size_t i = 0; i < NUM_ROWS; i++) {
        __clear_row(i);
    }
    col = 0;
    row = 0;
}

// TODO: ANSI color codes
void vga_putchar(char ch) {
    if (ch == '\n') {
        __print_newline();
        return;
    }

    if (col > NUM_COLS) {
        __print_newline();
    }

    vgaBuffer[col + NUM_COLS * row] = (struct vgachar) {
        .chr = (uint8_t)ch,
        .color = color
    };

    col++;
}

void vga_puts(char* str) {
    for (size_t i = 0; ;i++) {
        char character = (uint8_t) str[i];
        if (character == '\0')
            return;

        vga_putchar(character);
    }
}

void vga_color(uint8_t fg, uint8_t bg) {
    color = fg + (bg << 4);
}


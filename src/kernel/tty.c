#include "tty.h"

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

void __clear_row(size_t row) {
    struct vgachar empty = {
        .chr =  ' ',
        .color =  color
    };
    
    for (size_t col = 0; col < NUM_COLS; col++) {
        vgaBuffer[col + NUM_COLS*row] = empty;
    }
}

void __print_newline() {
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

void tty_clear() {
    for (size_t i = 0; i < NUM_ROWS; i++) {
        __clear_row(i);
    }
}

void tty_putchar(char ch) {
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

void tty_puts(char* str) {
    for (size_t i = 0; ;i++) {
        char character = (uint8_t) str[i];
        if (character == '\0')
            return;
        
        tty_putchar(character);
    }
}

void tty_color(uint8_t fg, uint8_t bg) {
    color = fg + (bg << 4);
}

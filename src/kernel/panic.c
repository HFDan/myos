#include "error.h"
#include "printk.h"
#include "drivers/video/vga.h"

struct regsOnStack {
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rbp;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;
    uint64_t rsp;
};

void u642hex(uint64_t num, char* out) {
    char hexDigits[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    out[0] = '0';
    out[1] = 'x';

    size_t addto = 2;
    while (num) {
        out[addto] = hexDigits[num%16];
        num /= 16;
        addto++;
    }

    out[addto+1] = '\0';
    size_t size = addto-2;

    char* out_remapped = &(out[0]) + 2;
    for (size_t i = 0; i < size/2; i++) {
        char aux = out_remapped[i];
        out_remapped[i] = out_remapped[size-i-1];
        out_remapped[size-i-1] = aux;
    }
}

__attribute__((noreturn))
void panic(const char* msg) {
    // Stop interrupts
    __asm volatile("cli");

    vga_clear();
    vga_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    printk("Kernel panic - not syncing: %s\n", msg);

    printk("Halting...\n");
    __asm volatile("hlt");
    
    __builtin_unreachable();
}

void kpanic(char* msg, void* regs) {
    vga_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    vga_puts("Begin Kernel Panic\n==================\n");
    vga_puts("Err: ");
    vga_puts(msg);
    vga_puts("\n");

    // Print status
    struct regsOnStack* stackRegs = (struct regsOnStack*)regs;

    vga_puts("Registers:\n");
    // i hate this
    #pragma region was ist das? aaaaaaああああああああああ
    {
        char out[20];

        u642hex(stackRegs->rsp, out);
        vga_puts("rsp: ");
        vga_puts(out);
        vga_puts("\n");

        u642hex(stackRegs->rax, out);
        vga_puts("rax: ");
        vga_puts(out);
        vga_puts("\n");

        u642hex(stackRegs->rbx, out);
        vga_puts("rbx: ");
        vga_puts(out);
        vga_puts("\n");

        u642hex(stackRegs->rcx, out);
        vga_puts("rcx: ");
        vga_puts(out);
        vga_puts("\n");

        u642hex(stackRegs->rdx, out);
        vga_puts("rdx: ");
        vga_puts(out);
        vga_puts("\n");

        u642hex(stackRegs->rsi, out);
        vga_puts("rsi: ");
        vga_puts(out);
        vga_puts("\n");

        u642hex(stackRegs->rdi, out);
        vga_puts("rdi: ");
        vga_puts(out);
        vga_puts("\n");

        u642hex(stackRegs->rbp, out);
        vga_puts("rbp: ");
        vga_puts(out);
        vga_puts("\n");

        u642hex(stackRegs->r8, out);
        vga_puts("r8:  ");
        vga_puts(out);
        vga_puts("\n");

        u642hex(stackRegs->r9, out);
        vga_puts("r9:  ");
        vga_puts(out);
        vga_puts("\n");

        u642hex(stackRegs->r10, out);
        vga_puts("r10: ");
        vga_puts(out);
        vga_puts("\n");

        u642hex(stackRegs->r11, out);
        vga_puts("r11: ");
        vga_puts(out);
        vga_puts("\n");

        u642hex(stackRegs->r12, out);
        vga_puts("r12: ");
        vga_puts(out);
        vga_puts("\n");

        u642hex(stackRegs->r13, out);
        vga_puts("r13: ");
        vga_puts(out);
        vga_puts("\n");

        u642hex(stackRegs->r14, out);
        vga_puts("r14: ");
        vga_puts(out);
        vga_puts("\n");

        u642hex(stackRegs->r15, out);
        vga_puts("r15: ");
        vga_puts(out);
        vga_puts("\n");
    }
    #pragma endregion

    __asm volatile("hlt");
}

void kerror(char* msg) {
    vga_color(VGA_COLOR_RED, VGA_COLOR_BLACK);
    vga_puts(msg);
    vga_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
}
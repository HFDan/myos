#include "error.h"
#include "tty.h"

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

void kpanic(char* msg, void* regs) {
    tty_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    tty_puts("Begin Kernel Panic\n==================\n");
    tty_puts("Err: ");
    tty_puts(msg);
    tty_puts("\n");
    
    // Print status
    struct regsOnStack* stackRegs = (struct regsOnStack*)regs;
    
    tty_puts("Registers:\n");
    // i hate this
    #pragma region was ist das? aaaaaaああああああああああ
    {
        char out[20];

        u642hex(stackRegs->rsp, out);
        tty_puts("rsp: ");
        tty_puts(out);
        tty_puts("\n");
        
        u642hex(stackRegs->rax, out);
        tty_puts("rax: ");
        tty_puts(out);
        tty_puts("\n");
        
        u642hex(stackRegs->rbx, out);
        tty_puts("rbx: ");
        tty_puts(out);
        tty_puts("\n");
        
        u642hex(stackRegs->rcx, out);
        tty_puts("rcx: ");
        tty_puts(out);
        tty_puts("\n");
        
        u642hex(stackRegs->rdx, out);
        tty_puts("rdx: ");
        tty_puts(out);
        tty_puts("\n");
        
        u642hex(stackRegs->rsi, out);
        tty_puts("rsi: ");
        tty_puts(out);
        tty_puts("\n");
        
        u642hex(stackRegs->rdi, out);
        tty_puts("rdi: ");
        tty_puts(out);
        tty_puts("\n");
        
        u642hex(stackRegs->rbp, out);
        tty_puts("rbp: ");
        tty_puts(out);
        tty_puts("\n");
        
        u642hex(stackRegs->r8, out);
        tty_puts("r8:  ");
        tty_puts(out);
        tty_puts("\n");
        
        u642hex(stackRegs->r9, out);
        tty_puts("r9:  ");
        tty_puts(out);
        tty_puts("\n");
        
        u642hex(stackRegs->r10, out);
        tty_puts("r10: ");
        tty_puts(out);
        tty_puts("\n");
        
        u642hex(stackRegs->r11, out);
        tty_puts("r11: ");
        tty_puts(out);
        tty_puts("\n");
        
        u642hex(stackRegs->r12, out);
        tty_puts("r12: ");
        tty_puts(out);
        tty_puts("\n");

        u642hex(stackRegs->r13, out);
        tty_puts("r13: ");
        tty_puts(out);
        tty_puts("\n");

        u642hex(stackRegs->r14, out);
        tty_puts("r14: ");
        tty_puts(out);
        tty_puts("\n");

        u642hex(stackRegs->r15, out);
        tty_puts("r15: ");
        tty_puts(out);
        tty_puts("\n");
    }
    #pragma endregion

    __asm volatile("hlt");
}

void kerror(char* msg) {
    tty_color(VGA_COLOR_RED, VGA_COLOR_BLACK);
    tty_puts(msg);
    tty_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
}
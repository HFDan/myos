#include <stdint.h>
#include "panic.h"

uintptr_t __stack_chk_guard = 0xf00dbabef00dbabe; // NOTE: THIS IS *VERY* NOT SECURE. __stack_chk_guard SHOULD BE RANDOMIZED, BUT I'M TOO LAZY TO DO IT YET
// TODO: Randomize __stack_chk_guard

__attribute__((noreturn))
void __stack_chk_fail() {
    panic("Stack smashing detected!\n");
}
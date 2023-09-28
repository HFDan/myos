#pragma once

void kerror(char* msg);
void kpanic(char* msg, void* addy);

__attribute__((noreturn))
void panic(const char* msg);
ASM := nasm
CC := clang
CXX := clang++
LD := ld
LDS := target/x86_64/link.ld

asm_source := $(shell find src/ -name *.asm)
asm_obj := $(patsubst src/%.asm, build/%.o, $(asm_source))

c_source := $(shell find src/ -name *.c)
c_obj := $(patsubst src/%.c, build/%.o, $(c_source))

cxx_source := $(shell find src/ -name *.cpp)
cxx_obj := $(patsubst src/%.cpp, build/%.o, $(cxx_source))

all_obj := $(c_obj) $(asm_obj) $(cxx_obj)

$(c_obj): build/%.o : src/%.c
	mkdir -p $(dir $@) && \
	$(CC) -c -I./include -ffreestanding --target=x86_64-pc-none $(patsubst build/%.o, src/%.c, $@) -o $@

$(cxx_obj): build/%.o : src/%.cpp
	mkdir -p $(dir $@) && \
	$(CXX) -c -I./include -ffreestanding --target=x86_64-pc-none $(patsubst build/%.o, src/%.cpp, $@) -o $@

$(asm_obj): build/%.o : src/%.asm
	mkdir -p $(dir $@) && \
	$(ASM) -f elf64 $(patsubst build/%.o, src/%.asm, $@) -o $@
	
.PHONY: build-amd64
build-amd64: $(all_obj)
	mkdir -p dist/x86_64 && \
	$(LD) -b elf64-x86-64 -n -o dist/x86_64/kernel.bin -T $(LDS) $(asm_obj) $(c_obj) $(cxx_obj) && \
	cp dist/x86_64/kernel.bin target/x86_64/iso/boot/kernel.bin && \
	grub-mkrescue /usr/lib/grub/i386-pc -o dist/x86_64/kernel.iso target/x86_64/iso

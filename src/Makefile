# Group 21B6 - kacchiOS Makefile 
CC = gcc
AS = nasm
LD = ld  

LDFLAGS = -m elf_i386 -T linker.ld
CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -I.

OBJ = boot.o kernel.o serial.o string.o \
      memory.o process.o scheduler.o

all: kacchiOS.bin

kacchiOS.bin: $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

boot.o: boot.s
	$(AS) -f elf32 $< -o $@

run: kacchiOS.bin
	qemu-system-i386 -kernel kacchiOS.bin -serial stdio

clean:
	rm -f *.o kacchiOS.bin

.PHONY: all clean run

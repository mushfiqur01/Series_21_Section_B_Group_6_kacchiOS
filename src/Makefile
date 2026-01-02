# Group 21B6 - kacchiOS Makefile
CC = i686-elf-gcc
AS = nasm
LDFLAGS = -T src/linker.ld -ffreestanding -O2 -nostdlib
CFLAGS = -ffreestanding -O2 -Wall -Wextra -Isrc

OBJ = src/boot.o src/kernel.o src/serial.o src/string.o \
      src/memory.o src/process.o src/scheduler.o

kacchiOS.bin: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) -lgcc

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

src/boot.o: src/boot.s
	$(AS) -f elf32 $< -o $@

clean:
	rm -f src/*.o kacchiOS.bin
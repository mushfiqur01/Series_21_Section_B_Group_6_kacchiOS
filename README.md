# kacchiOS

A simple operating system kernel developed from scratch as part of an academic project for Series 21, Section B - Group 6.

## Overview

kacchiOS is a minimal operating system kernel that demonstrates fundamental OS concepts including memory management, process handling, and CPU scheduling. The project is written primarily in C with assembly for low-level boot operations.

## Features

- **Boot Loader**: Custom bootloader written in assembly (`boot.s`)
- **Memory Management**: Dynamic memory allocation and management system
- **Process Management**: Process creation, termination, and context switching
- **Scheduler**: CPU scheduling implementation for multitasking
- **Serial Communication**: Serial port I/O for debugging and output
- **String Utilities**: Basic string manipulation functions

## Project Structure

```
kacchiOS/
├── src/
│   ├── boot.s          # Assembly bootloader
│   ├── kernel.c        # Main kernel entry point
│   ├── memory.c/h      # Memory management
│   ├── process.c/h     # Process management
│   ├── scheduler.c/h   # CPU scheduler
│   ├── serial.c/h      # Serial port communication
│   ├── string.c/h      # String utilities
│   ├── types.h         # Type definitions
│   ├── linker.ld       # Linker script
│   └── Makefile        # Build configuration
├── docs/               # Documentation
├── video/              # Demo videos
└── README.md
```

## Building

To build kacchiOS, you need:
- GCC cross-compiler for i386/i686
- GNU Make
- NASM or similar assembler

Build the kernel:
```bash
cd src
make
```

## Running

You can run kacchiOS using an emulator like QEMU:
```bash
qemu-system-i386 -kernel kernel.bin
```

Or create a bootable image for testing on real hardware or virtualization software.

## Development Team

**Series 21, Section B - Group 6**

## License

See [LICENSE](LICENSE) for details.

## Acknowledgments

This project was developed as part of an operating systems course to learn and implement core OS concepts from the ground up.

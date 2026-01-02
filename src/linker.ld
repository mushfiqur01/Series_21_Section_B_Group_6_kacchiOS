ENTRY(_start)

SECTIONS {
    /* The kernel should be loaded at 1MB */
    . = 1M;

    .text BLOCK(4K) : ALIGN(4K) {
        *(.multiboot)   /* Multiboot header must be at the start */
        *(.text)
    }

    .rodata BLOCK(4K) : ALIGN(4K) {
        *(.rodata)
    }

    .data BLOCK(4K) : ALIGN(4K) {
        *(.data)
    }

    .bss BLOCK(4K) : ALIGN(4K) {
        *(COMMON)
        *(.bss)
    }
}
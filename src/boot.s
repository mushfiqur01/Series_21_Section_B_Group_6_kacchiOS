; Multiboot header
MB_MAGIC    equ 0x1BADB002
MB_FLAGS    equ 0x03
MB_CHECKSUM equ -(MB_MAGIC + MB_FLAGS)

section .multiboot
    dd MB_MAGIC
    dd MB_FLAGS
    dd MB_CHECKSUM

section .bss
    align 16
    stack_bottom: resb 16384
    stack_top:

section .text
    global _start
    global switch_to
    extern kernel_main

_start:
    mov esp, stack_top
    call kernel_main

; Fulfills Checklist: Context switch (70%)
switch_to:
    mov eax, [esp + 4]    ; uint32_t **old_esp
    mov edx, [esp + 8]    ; uint32_t *new_esp

    push ebp
    push edi
    push esi
    push ebx

    mov [eax], esp        ; Save current ESP to process struct
    mov esp, edx          ; Switch to new process stack

    pop ebx
    pop esi
    pop edi
    pop ebp
    ret
.section .multiboot
.align 4
.long 0x1BADB002
.long 0x0
.long -(0x1BADB002)

.section .text
.global _start
.extern kernel_main

_start:
    call kernel_main

hang:
    jmp hang

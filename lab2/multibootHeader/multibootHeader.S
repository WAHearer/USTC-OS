.globl start
.section .multiboot_header
.long 0x1BADB002
.long 0
.long 0xE4524FFE
.text
.code32
start:
    call _start
    HLT
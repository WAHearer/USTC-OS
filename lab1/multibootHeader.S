.globl start
.section .multiboot_header
.long 0x1BADB002
.long 0
.long 0xE4524FFE
.text
.code32
start:
    movl $0x2f652f48,0xb8000
    movl $0x2f6c2f6c,0xb8004
    movl $0x2f202f6f,0xb8008
    movl $0x2f6f2f57,0xb800C
    movl $0x2f6c2f72,0xb8010
    movl $0x2f212f64,0xb8014
    movl $0x2f422f50,0xb8018
    movl $0x2f332f32,0xb801C
    movl $0x2f302f30,0xb8020
    movl $0x2f322f30,0xb8024
    movl $0x2f332f35,0xb8028
    movl $0x2f742f5f,0xb802C
    movl $0x2f7a2f68,0xb8030
    movl $0x2f612f68,0xb8034
    movl $0x2f672f6e,0xb8038

    movw $0x3f8 , %dx
    movb $0x48 , %al
    outb %al , %dx
    movw $0x3f8 , %dx
    movb $0x65 , %al
    outb %al , %dx
    movw $0x3f8 , %dx
    movb $0x6c , %al
    outb %al , %dx
    movw $0x3f8 , %dx
    movb $0x6c , %al
    outb %al , %dx
    movw $0x3f8 , %dx
    movb $0x6f , %al
    outb %al , %dx
    HLT
	.arch armv8-a
	.file	"main.c"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
.LFB0:
	mov x0, #0
    movk x0, 0xffff, lsl 48
    movk x0, 0xffff, lsl 32
    movk x0, 0xffff, lsl 16
    movk x0, 0xffff, lsl 0
    mov x1, #-1
    mov x2, #0xFFFFFFFFF
	ret
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
	.section	.note.GNU-stack,"",@progbits

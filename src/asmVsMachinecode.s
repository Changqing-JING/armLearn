.global lab_memset

lab_memset:
	.cfi_startproc
	mov x0, #0
    movk x0, 0xffff, lsl 48
    movk x0, 0xffff, lsl 32
    movk x0, 0xffff, lsl 16
    movk x0, 0xffff, lsl 0
    mov x1, #-1
    mov x2, #0xFFFFFFFFF

    mov x5, #0xDDEE
	ret
	.cfi_endproc

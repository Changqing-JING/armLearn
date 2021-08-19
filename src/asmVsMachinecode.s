.global lab_memset

byte_memset:
    1: //loop
    cmp x2, 0
    beq .byte_memset_end
    strb w1, [x0], 1
    sub x2, x2, 1
    b 1b
    .byte_memset_end:
    ret

lab_memset:
    cmp x2, 32
    bge .32
    mov x28, x30
    bl byte_memset
    mov x30, x28
    
    

    .32: //more_than_32
    and x5, x0, 0xF
    mov x7, 16
    sub x6, x7, x5

    mov x20, x0
    mov x22, x2

    mov x28, x30

    mov x2, x6

    bl byte_memset

    mov x30, x28

    mov x2, x22
    mov x0, x20

    add x0, x0, x6
    sub x2, x2, x6

    lsl x6, x1, 8
    orr x6, x6, x1
    lsl x7, x6, 16
    orr x7, x7, x6
    lsl x6, x7, 32
    orr x6, x6, x7

    1: //loop
    cmp x2, 16
    blt .19
    stp x6, x6, [x0], 16
    sub x2, x2, 16
    b 1b


    .19:
    mov x28, x30

    bl byte_memset

    mov x30, x28

    .20: //end
        ret

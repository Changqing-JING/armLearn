.global lab_memset
.global byte_memset //Let linker find this function
byte_memset:
    1: //loop
    cmp x2, 0 //compare x2 with 0
    beq .byte_memset_end //conditional Jump: jump to a label if equal or greater, otherwise contienue
    strb w1, [x0], 1 //store 1 byte value of w1 to memory address x1, then x0 = x0 + 1
    sub x2, x2, 1 //x2 = x2 - 1
    b 1b //unconditional jmp to label
    .byte_memset_end:
    ret //return

lab_memset:
    sub sp, sp, 32 //alloc 32 bytes stack, in arm64 stack grows downwards.
    cmp x2, 32
    bge .32
    //still copy byte by byte if the size is very small
    str x30, [sp] //save return address of current function to stack
    bl byte_memset //jump to byte_memset and save return address of byte_memset to X30
    ldr x30, [sp] //restore return address to X30
    b .20
 //------------------------   

    .32: //more_than_32
    //calculate unaligned size
    and x5, x0, 0xF //x5 = x0 & 0xF
    mov x7, 16 //x7 = 16
    sub x6, x7, x5 //x6 = x7 - x5, x6 is unaligned size
//-------------------------------------------
    //store local variables and prepare paramters
    str x0, [sp] //x0 is volatile register which need to be saved by caller
    str x2, [sp, 8] // store x2 to sp + 8

    str x30, [sp, 16]

    mov x2, x6

    bl byte_memset

    //restore local variables
    ldr x0, [sp]
    ldr x2, [sp, 8]

    ldr x30, [sp, 16]
//-------------------
    //calculate aligned address and rest size
    add x0, x0, x6 //x0 = x0 + x6
    sub x2, x2, x6 //x2 = x2 - x6

    //fill x6 with value of x1, for example: x1=0x10, then x6 = 0x1010101010101010
    lsl x6, x1, 8 //x6 = x1 << 8
    orr x6, x6, x1 //x6 = x6 | x1
    lsl x7, x6, 16
    orr x7, x7, x6
    lsl x6, x7, 32
    orr x6, x6, x7

    1: //loop
    cmp x2, 16
    blt .19 //jump if less
    stp x6, x6, [x0], 16 //store x6 to memory address x0, store x6 to memory [x0+8], x0 = x0 + 16
    sub x2, x2, 16
    b 1b

   //copy rest bytes
    .19:
    mov x28, x30

    bl byte_memset

    mov x30, x28

    .20: //end
    //recovery stack
    add sp, sp, 32
    ret

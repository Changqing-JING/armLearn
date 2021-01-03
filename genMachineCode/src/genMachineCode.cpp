#include "printMachineCode.h"
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include "Arrch64Register.h"

namespace  MoveWideImmediate{
    enum SF{
        bit32=0,
        bit64=0b10000000'00000000'00000000'00000000,
    };
    enum OPC{
        MOVN = 0,
        MOVZ = 0b10'00000000'00000000'00000000'00000,
        MOVK = 0b11'00000000'00000000'00000000'00000,
    };
    

    
    const uint8_t registerMask = 0b1'1111;
    const uint32_t imm16Mask = 0b11111111'11111111'00000;
    const uint8_t imm16Offset = 5;
    const uint8_t hwOffset = imm16Offset + sizeof(uint16_t)*8;
    const uint32_t opCode = 0b100101'00'00000000'00000000'00000;

    static inline void emmitMoveImmediateValue(Aarch64Register reg, uint16_t imm16, uint8_t hwBase, OPC opc, SF sf, uint32_t* instruction){
        *instruction = opCode;
        *instruction |= (reg&registerMask);
        *instruction |= ((imm16<<imm16Offset)&imm16Mask);
        *instruction |= (hwBase<<hwOffset);
        //*instruction |= opCode; set opcode is not necessary here, keep as comment for better readability
        *instruction |= opc;
        *instruction |= sf;
    }
};



int emmitMoveImmediateValue64(Aarch64Register reg, uint64_t value, uint32_t*& mc){

    if((~value)<=UINT16_MAX){
        MoveWideImmediate::emmitMoveImmediateValue(reg, ~(value), 0, MoveWideImmediate::OPC::MOVN,  MoveWideImmediate::SF::bit64, mc);
        mc++;
        return 0;
    }
    
    MoveWideImmediate::emmitMoveImmediateValue(reg, value&UINT16_MAX, 0, MoveWideImmediate::OPC::MOVZ,  MoveWideImmediate::SF::bit64, mc);
    mc++;
    uint8_t hw = 1;

    while((value = value>>16)>0){

        MoveWideImmediate::emmitMoveImmediateValue(reg, value&UINT16_MAX, hw, MoveWideImmediate::OPC::MOVK,  MoveWideImmediate::SF::bit64, mc);
        hw++;
        mc++;
    }
    
    return 0;
}

int main(){
    
    
    uint32_t* machineCodeStart = (uint32_t*)malloc(1024);
    uint32_t* machineCode = machineCodeStart;

    emmitMoveImmediateValue64(Aarch64Register::x5, 0x11223344556677LL, machineCode);
    emmitMoveImmediateValue64(Aarch64Register::x6, -1, machineCode);
    uint32_t machineCodeSize = (uintptr_t)machineCode - (uintptr_t)machineCodeStart;
    dumpDisassembly((uint8_t*)machineCodeStart, machineCodeSize);


    return 0;
}
#include "printMachineCode.h"
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include "Arrch64Register.h"
#include <type_traits>

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
    

    
    static const uint8_t registerMask = 0b1'1111;
    static const uint32_t imm16Mask = 0b11111111'11111111'00000;
    static const uint8_t imm16Offset = 5;
    static const uint8_t hwOffset = imm16Offset + sizeof(uint16_t)*8;
    static const uint32_t opCode = 0b100101'00'00000000'00000000'00000;

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


template<class T>
int emmitMoveImmediateValue(Aarch64Register reg, T value, uint32_t*& mc){
    static_assert(std::is_same<T, uint32_t>::value || std::is_same<T, uint64_t>::value|| std::is_same<T, uint16_t>::value|| std::is_same<T, uint8_t>::value,
                "only accept uint8 uint16 uint32 and uint64 in mov");
    MoveWideImmediate::SF sf;

    if (sizeof(T)<=4){
        sf = MoveWideImmediate::SF::bit32;
    }else{
        sf = MoveWideImmediate::SF::bit64;
    }

    if((~value)<=UINT16_MAX){
        MoveWideImmediate::emmitMoveImmediateValue(reg, ~(value), 0, MoveWideImmediate::OPC::MOVN,  sf, mc);
        mc++;
        return 0;
    }
    
    MoveWideImmediate::emmitMoveImmediateValue(reg, value&UINT16_MAX, 0, MoveWideImmediate::OPC::MOVZ,  sf, mc);
    mc++;
    uint8_t hw = 1;

    while((value = value>>16)>0){

        MoveWideImmediate::emmitMoveImmediateValue(reg, value&UINT16_MAX, hw, MoveWideImmediate::OPC::MOVK,  sf, mc);
        hw++;
        mc++;
    }
    
    return 0;
}

int main(){
    
    
    uint32_t* machineCodeStart = (uint32_t*)malloc(1024);
    uint32_t* machineCode = machineCodeStart;

    emmitMoveImmediateValue(Aarch64Register::x5, 0x11223344556677UL, machineCode);
    emmitMoveImmediateValue(Aarch64Register::x6, (uint32_t)-1, machineCode);
    uint32_t machineCodeSize = (uintptr_t)machineCode - (uintptr_t)machineCodeStart;
    dumpDisassembly((uint8_t*)machineCodeStart, machineCodeSize);


    return 0;
}
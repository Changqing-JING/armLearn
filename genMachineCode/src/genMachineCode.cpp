#include "printMachineCode.h"
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include "Arrch64Register.h"

struct __attribute__((packed))  MoveWideImmediate{
    static const uint16_t OPCODEDEF = 0b100101;
    enum SF{
        bit32=0,
        bit64=1,
    };
    enum OPC{
        MOVN = 0b00,
        MOVZ = 0b10,
        MOVK = 0b11,
    };
    
    Aarch64Register Rd:5;
    uint16_t imm16:16;
    uint8_t hw:2;
    uint16_t opCode:6;
    OPC opc:2;
    SF sf:1;

    static inline void emmitMoveImmediateValue(Aarch64Register reg, uint16_t imm16, uint8_t hw, OPC opc, SF sf, MoveWideImmediate* instruction){
        instruction->Rd = reg;
        instruction->imm16 = imm16;
        instruction->hw = hw;
        instruction->opCode = OPCODEDEF;
        instruction->opc = opc;
        instruction->sf = sf;
    }
};



int emmitMoveImmediateValue64(Aarch64Register reg, uint64_t value, uint32_t*& mc){
    MoveWideImmediate* mov_ins = (MoveWideImmediate*)mc;
    if((~value)<=UINT16_MAX){
        MoveWideImmediate::emmitMoveImmediateValue(reg, ~(value), 0, MoveWideImmediate::OPC::MOVN,  MoveWideImmediate::SF::bit64, mov_ins);
        mc++;
        return 0;
    }
    
    MoveWideImmediate::emmitMoveImmediateValue(reg, value&UINT16_MAX, 0, MoveWideImmediate::OPC::MOVZ,  MoveWideImmediate::SF::bit64, mov_ins);
    mc++;
    uint8_t hw = 1;

    while((value = value>>16)>0){
        MoveWideImmediate* mov_ins = (MoveWideImmediate*)mc;
        MoveWideImmediate::emmitMoveImmediateValue(reg, value&UINT16_MAX, hw, MoveWideImmediate::OPC::MOVK,  MoveWideImmediate::SF::bit64, mov_ins);
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
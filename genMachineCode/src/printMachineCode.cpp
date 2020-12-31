#include "printMachineCode.h"
#include <stdio.h>
#include <stdlib.h>

void dumpDisassembly(uint8_t* start, uint32_t length){
    #ifndef __OPTIMIZE__ 
        FILE* pf = fopen("mc_debug.bin", "wb");

        fwrite(start, 1, length, pf);

        fclose(pf);
        fflush(stdout);
        system("aarch64-linux-gnu-objdump -b binary -D -m aarch64  mc_debug.bin");
    #endif
}
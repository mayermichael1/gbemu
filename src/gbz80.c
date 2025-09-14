#include "include/gbz80.h"

void 
init_gbz_emulator()
{
    instructions[0x00]  = 
    (gb_instruction)
    {
        .operation = GB_OPERATION_NOP,
        .cycles = 4,
        .instruction_size = 1
    };

    instructions[0x09] = 
    (gb_instruction)
    {
        .operation = GB_OPERATION_ADD,
        .cycles = 8,
        .instruction_size = 1,
    };
}


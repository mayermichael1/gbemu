#include "include/gbz80.h"

/// OPERATIONS

GB_OPERATION(gb_operation_stub)
{
    ASSERT(true);
}

GB_OPERATION(gb_nop)
{
}

GB_OPERATION(gb_add)
{
    
}

void 
init_gbz_emulator()
{
    instructions[0x00]  = 
    (gb_instruction)
    {
        .op = gb_nop,
        .cycles = 4,
        .instruction_size = 1
    };

    instructions[0x09] = 
    (gb_instruction)
    {
        .op = gb_add,
        .cycles = 8,
        .instruction_size = 1,
    };

    for(u8 i = 0; i < 255; ++i)
    {
        if(instructions[i].op == 0)
        {
            instructions[i].op = gb_operation_stub;
        }
    }
}





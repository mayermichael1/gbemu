#include "lib/c_test_suite/include/test_suite.h"

#include "include/general.h"

#include "include/platform.h"
#include "src/linux_platform.c"

#include "include/memory.h"

#include "include/gbz80.h"
#include "src/gbz80.c"

UNIT()
{
    b8 success = false;
    init_gbz_emulator();
    temp_memory gb_mem_arena = create_temp_memory(sizeof(gb_state));
    gb_state *gstate = (gb_state*)temp_memory_push(&gb_mem_arena, sizeof(gb_state)); 

    gstate->reg.PC = OFFSET_OF(gb_memory, rom00);
    gstate->ram.rom00[0] = 0x09;
    gstate->ram.rom00[1] = 0x09;
    gstate->ram.rom00[2] = 0x09;
    gstate->reg.HL = 1;
    gstate->reg.BC = 2;


    for(u64 cycle = 0; cycle < 100; ++cycle)
    {
        gb_perform_cycle(gstate);
        printf("cycle: %ld \tHL: %d \t\n", gstate->cycle, gstate->reg.HL);
    }

    success = gstate->reg.HL == 7;

    destroy_temp_memory(&gb_mem_arena);
    return(success);
}

UNIT()
{
    b8 success = false;
    init_gbz_emulator();
    temp_memory gb_mem_arena = create_temp_memory(sizeof(gb_state));
    gb_state *gstate = (gb_state*)temp_memory_push(&gb_mem_arena, sizeof(gb_state)); 

    gstate->reg.PC = OFFSET_OF(gb_memory, rom00);
    gstate->ram.rom00[0] = 0x86;
    gstate->reg.HL = 15;

    for(u64 cycle = 0; cycle < 4; ++cycle)
    {
        gb_perform_cycle(gstate);
        printf("cycle: %ld \tHL: %d \t\n", gstate->cycle, gstate->reg.HL);
    }

    success = gstate->current_instruction.operand_b.value16 == OFFSET_OF(gb_register, HL) / 2; 
    return(success);
}

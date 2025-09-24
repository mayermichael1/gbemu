#include "lib/c_test_suite/include/test_suite.h"

#include "include/general.h"

#include "include/platform.h"
#include "src/linux_platform.c"

#include "include/memory.h"

#include "include/gbz80.h"
#include "src/gbz80.c"

#define GB_SETUP \
    b8 success = false; \
    init_gbz_emulator(); \
    temp_memory gb_mem_arena = create_temp_memory(sizeof(gb_state)); \
    gb_state *gstate = (gb_state*)temp_memory_push(&gb_mem_arena, sizeof(gb_state)); \
    gstate->reg.PC = OFFSET_OF(gb_memory, rom00);

#define RET \
    destroy_temp_memory(&gb_mem_arena); \
    return(success);

void
load_instructions(gb_state *state, u8 opcode, u32 count)
{
    for(u32 i=0; i<count; ++i)
    {
        state->ram.rom00[i] = opcode;
    }
}

void
perform_cycles(gb_state *state, u32 cycles)
{

    for(u64 cycle = 0; cycle < cycles ; ++cycle)
    {
        gb_perform_cycle(state);
    }
}


/// 0x09   ADD HL, BC
UNIT()
{
    GB_SETUP

    gstate->reg.HL = 1;
    gstate->reg.BC = 2;

    load_instructions(gstate, 0x09, 3);
    perform_cycles(gstate, 3 * 8);

    success = gstate->reg.HL == 7; 

    RET
}

UNIT()
{
    GB_SETUP

    gstate->reg.HL = 1;
    gstate->reg.BC = (u8)-2;

    load_instructions(gstate, 0x09, 3);
    perform_cycles(gstate, 3 * 8);
    
    success = (s8)gstate->reg.HL == -5; 

    RET
}

/// 0x19    ADD HL, DE
UNIT()
{
    GB_SETUP

    gstate->reg.HL = 1;
    gstate->reg.DE = 2;

    load_instructions(gstate, 0x19, 3);
    perform_cycles(gstate, 3 * 8);
    
    success = (s8)gstate->reg.HL == 7; 

    RET
}

UNIT()
{
    GB_SETUP

    gstate->reg.HL = 1;
    gstate->reg.DE = (u8)-2;

    load_instructions(gstate, 0x19, 3);
    perform_cycles(gstate, 3 * 8);
    
    success = (s8)gstate->reg.HL == -5; 

    RET
}

/// 0x29    ADD HL, HL
UNIT()
{
    GB_SETUP

    gstate->reg.HL = 1;

    load_instructions(gstate, 0x29, 3);
    perform_cycles(gstate, 3 * 8);
    
    success = (s8)gstate->reg.HL == 8 ;

    RET
}

UNIT()
{
    GB_SETUP

    gstate->reg.HL = -1;

    load_instructions(gstate, 0x29, 3);
    perform_cycles(gstate, 3 * 8);
    
    success = (s8)gstate->reg.HL == -8; 

    RET
}

/// 0x39    ADD HL, SP

UNIT()
{
    GB_SETUP

    gstate->reg.HL = 1;
    gstate->reg.SP = 2;

    load_instructions(gstate, 0x39, 3);
    perform_cycles(gstate, 3 * 8);
    
    success = (s8)gstate->reg.HL == 7;

    RET
}

UNIT()
{
    GB_SETUP

    gstate->reg.HL = 1;
    gstate->reg.SP = -2;

    load_instructions(gstate, 0x39, 3);
    perform_cycles(gstate, 3 * 8);
    
    success = (s8)gstate->reg.HL == -5; 

    RET
}
/// 0x80    ADD A, B 
UNIT()
{
    GB_SETUP

    gstate->reg.A = 1;
    gstate->reg.B = 2;

    load_instructions(gstate, 0x80, 3);
    perform_cycles(gstate, 3 * 4);
    
    success = (s8)gstate->reg.A == 7;

    RET
}

UNIT()
{
    GB_SETUP

    gstate->reg.A = 1;
    gstate->reg.B = -2;

    load_instructions(gstate, 0x80, 3);
    perform_cycles(gstate, 3 * 4);
    
    success = (s8)gstate->reg.A == -5; 

    RET
}

/// 0x81    ADD A, C

UNIT()
{
    GB_SETUP

    gstate->reg.A = 1;
    gstate->reg.C = 2;

    load_instructions(gstate, 0x81, 3);
    perform_cycles(gstate, 3 * 4);
    
    success = (s8)gstate->reg.A == 7;

    RET
}

UNIT()
{
    GB_SETUP

    gstate->reg.A = 1;
    gstate->reg.C = -2;

    load_instructions(gstate, 0x81, 3);
    perform_cycles(gstate, 3 * 4);
    
    success = (s8)gstate->reg.A == -5; 

    RET
}

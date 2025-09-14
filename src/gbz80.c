#include "include/gbz80.h"

#include "include/general.h"

void 
init_gbz_emulator()
{
    instructions[0x00]  = 
    (gb_instruction)
    {
        .operation = GB_OPERATION_NOP,
        .cycles = 4,
    };

    instructions[0x09] = 
    (gb_instruction)
    {
        .operation = GB_OPERATION_ADD,
        .cycles = 8,
        .operand_a = 
        {
            .type = GB_OPERAND_REGISTER,
            .value8 = (OFFSET_OF(gb_register, HL) / 2)
        },
        .operand_b = 
        {
            .type = GB_OPERAND_REGISTER,
            .value8 = (OFFSET_OF(gb_register, BC) / 2)
        }
    };
}

void 
gb_perform_instruction(gb_state *state)
{
    //TODO: this will not work for conditional cycle counts
    u64 delta_cycles = state->cycle - state->last_operation_cycle;
    if(delta_cycles >= state->current_instruction.cycles)
    {
        switch(state->current_instruction.operation)
        {
            case GB_OPERATION_NOP:
            {
            }
            break;
            case GB_OPERATION_ADD:
            {
                //TODO: this changes depending if register 16 or 8 is used
                if( state->current_instruction.operand_a.type == GB_OPERAND_REGISTER &&
                    state->current_instruction.operand_b.type == GB_OPERAND_REGISTER
                )
                {
                    state->reg.registers_wide[state->current_instruction.operand_a.value8] = 
                    state->reg.registers_wide[state->current_instruction.operand_a.value8] + 
                    state->reg.registers_wide[state->current_instruction.operand_b.value8];
                }
            }
            break;
            default:
            {
            }
            break;
        }
        state->current_instruction = (gb_instruction){0};
        state->last_operation_cycle = state->cycle;
    }
}

void
gb_load_next_instruction(gb_state *state)
{
    u8 opcode = state->ram.bytes[state->reg.PC];
    gb_instruction instruction = instructions[opcode];

    if(instruction.additional_bytes)
    {
        //TODO: if the instruction size is > 1 additional data has to be loaded
    }

    //TODO: should PC be increased here already? 
    //
    state->reg.PC += 1 + instruction.additional_bytes;
    state->current_instruction = instruction;
}

void
gb_perform_cycle(gb_state *state)
{
    state->cycle++;

    if(!state->current_instruction.operation)
    {
       gb_load_next_instruction(state);
    }

    if(state->current_instruction.operation)
    {
       gb_perform_instruction(state);
    }
}

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
    gb_instruction instruction = state->current_instruction;
    gb_operand operand_a = instruction.operand_a;
    gb_operand operand_b = instruction.operand_b;
    gb_register *reg = &state->reg;
    gb_memory *ram = &state->ram;

    if( instruction.operation &&
        delta_cycles >= instruction.cycles)
    {
        switch(instruction.operation)
        {
            case GB_OPERATION_NOP:
            {
            }
            break;
            case GB_OPERATION_ADD:
            {
                //TODO: this changes depending if register 16 or 8 is used
                if( operand_a.type == GB_OPERAND_REGISTER &&
                    operand_b.type == GB_OPERAND_REGISTER
                )
                {
                    reg->registers_wide[operand_a.value8] = 
                    reg->registers_wide[operand_a.value8] + 
                    reg->registers_wide[operand_b.value8];
                }
            }
            break;
            default:
            {
            }
            break;
        }
        state->current_instruction= (gb_instruction){0};
        state->last_operation_cycle = state->cycle;
    }
}

void
gb_load_next_instruction(gb_state *state)
{
    if(!state->current_instruction.operation)
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
}

void
gb_perform_cycle(gb_state *state)
{
    state->cycle++;

    //NOTE: currently instruction load and perform will never happen on the same 
    //      cycle. This could be a if / else  probably
   gb_load_next_instruction(state);
   gb_perform_instruction(state);
}

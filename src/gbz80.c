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
        .instruction_size = 1
    };

    instructions[0x09] = 
    (gb_instruction)
    {
        .operation = GB_OPERATION_ADD,
        .cycles = 8,
        .instruction_size = 1,
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
gb_perform_instruction(gb_state *state, gb_instruction instruction)
{
    //NOTE: this could be make to work with 0 initialzied value
    //TODO: should this happen at the end of the instruction?
    state->reg.PC += instruction.instruction_size;

    switch(instruction.operation)
    {
        case GB_OPERATION_NOP:
        {
        }
        break;
        case GB_OPERATION_ADD:
        {
            //TODO: this changes depending if register 16 or 8 is used
            if( instruction.operand_a.type == GB_OPERAND_REGISTER &&
                instruction.operand_b.type == GB_OPERAND_REGISTER
            )
            {
                state->reg.registers_wide[instruction.operand_a.value8] = 
                state->reg.registers_wide[instruction.operand_a.value8] + 
                state->reg.registers_wide[instruction.operand_b.value8];
            }
        }
        break;
        default:
        {
        }
        break;
    }
}


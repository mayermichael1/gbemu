#include "include/gbz80.h"

#include "include/general.h"


#define REG16INDEX(registers, reg) (OFFSET_OF(registers, reg) / 2)

#define REG8INDEX(registers, reg) (OFFSET_OF(registers, reg))

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
            .value16 = REG16INDEX(gb_register, HL),
            .wide = true,
        },
        .operand_b = 
        {
            .type = GB_OPERAND_REGISTER,
            .value16 = REG16INDEX(gb_register, BC),
            .wide = true,
        }
    };

    instructions[0x86] = 
    (gb_instruction)
    {
        .operation = GB_OPERATION_ADD,
        .cycles = 8,
        .operand_a = 
        {
            .type = GB_OPERAND_REGISTER,
            .value8 = REG8INDEX(gb_register, A),
        },
        .operand_b = 
        {
            .type = GB_OPERAND_REGISTER_ADDRESS,
            .value16 = REG16INDEX(gb_register, HL),
            .wide = true,
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
                    reg->registers_wide[operand_a.value16] = 
                    reg->registers_wide[operand_a.value16] + 
                    reg->registers_wide[operand_b.value16];
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

b8 
operand_needs_more_bytes(gb_operand operand)
{
    b8 more_bytes = false;
    if( operand.type == GB_OPERAND_ADDRESS || 
        operand.type == GB_OPERAND_IMMEDIATE)
    {
        more_bytes = true;
    }
    return(more_bytes);
}

void
read_operand_bytes(gb_state *state, gb_operand *operand)
{
    if(operand->wide)
    {
        operand->value16 = *(u16*)&(state->ram.bytes[state->reg.PC+1]);
    }
    else
    {
        operand->value8 = state->ram.bytes[state->reg.PC+1];
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
            // TODO: maybe this should be unified in some way
            if(operand_needs_more_bytes(instruction.operand_a))
            {
                read_operand_bytes(state, &instruction.operand_a);
            }
            if(operand_needs_more_bytes(instruction.operand_b))
            {
                read_operand_bytes(state, &instruction.operand_b);
            }
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

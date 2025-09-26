#include "include/gbz80.h"

#include "include/general.h"

#define REG16INDEX(registers, reg) (OFFSET_OF(registers, reg) / 2)

#define REG8INDEX(registers, reg) (OFFSET_OF(registers, reg))

void 
set_zero_flag(gb_register *reg)
{
    reg->F = SET_BIT(reg->F, 7);
}

void 
unset_zero_flag(gb_register *reg)
{
    reg->F = UNSET_BIT(reg->F, 7);
}

void 
set_half_carry_flag(gb_register *reg)
{
    reg->F = SET_BIT(reg->F, 5);
}

void 
unset_half_carry_flag(gb_register *reg)
{
    reg->F = UNSET_BIT(reg->F, 5);
}

void 
set_carry_flag(gb_register *reg)
{
    reg->F = SET_BIT(reg->F, 4);
}

void 
unset_carry_flag(gb_register *reg)
{
    reg->F = UNSET_BIT(reg->F, 4);
}


void 
init_gbz_emulator()
{
    instructions[0x00]  = 
    (gb_instruction)
    {
        .operation = GB_OPERATION_NOP,
        .cycles = 4,
    };

    /// ADD instructions

    instructions[0x09] = 
    (gb_instruction)
    {
        .operation = GB_OPERATION_ADD,
        .cycles = 8,
        .destination = 
        {
            .type = GB_OPERAND_REGISTER,
            .value16 = REG16INDEX(gb_register, HL),
            .wide = true,
        },
        .source = 
        {
            .type = GB_OPERAND_REGISTER,
            .value16 = REG16INDEX(gb_register, BC),
            .wide = true,
        },
        .zero_flag = GB_FLAG_ACTION_LEAVE,
        .subtract_flag = GB_FLAG_ACTION_UNSET,
        .half_carry_flag = GB_FLAG_ACTION_ACCORDINGLY,
        .carry_flag = GB_FLAG_ACTION_ACCORDINGLY,
    };

    instructions[0x19] = 
    (gb_instruction)
    {
        .operation = GB_OPERATION_ADD,
        .cycles = 8,
        .destination = 
        {
            .type = GB_OPERAND_REGISTER,
            .value16 = REG16INDEX(gb_register, HL),
            .wide = true,
        },
        .source = 
        {
            .type = GB_OPERAND_REGISTER,
            .value16 = REG16INDEX(gb_register, DE),
            .wide = true,
        },
        .zero_flag = GB_FLAG_ACTION_LEAVE,
        .subtract_flag = GB_FLAG_ACTION_UNSET,
        .half_carry_flag = GB_FLAG_ACTION_ACCORDINGLY,
        .carry_flag = GB_FLAG_ACTION_ACCORDINGLY,
    };
    
    instructions[0x29] = 
    (gb_instruction)
    {
        .operation = GB_OPERATION_ADD,
        .cycles = 8,
        .destination = 
        {
            .type = GB_OPERAND_REGISTER,
            .value16 = REG16INDEX(gb_register, HL),
            .wide = true,
        },
        .source = 
        {
            .type = GB_OPERAND_REGISTER,
            .value16 = REG16INDEX(gb_register, HL),
            .wide = true,
        },
        .zero_flag = GB_FLAG_ACTION_LEAVE,
        .subtract_flag = GB_FLAG_ACTION_UNSET,
        .half_carry_flag = GB_FLAG_ACTION_ACCORDINGLY,
        .carry_flag = GB_FLAG_ACTION_ACCORDINGLY,
    };

    instructions[0x39] = 
    (gb_instruction)
    {
        .operation = GB_OPERATION_ADD,
        .cycles = 8,
        .destination = 
        {
            .type = GB_OPERAND_REGISTER,
            .value16 = REG16INDEX(gb_register, HL),
            .wide = true,
        },
        .source = 
        {
            .type = GB_OPERAND_REGISTER,
            .value16 = REG16INDEX(gb_register, SP),
            .wide = true,
        },
        .zero_flag = GB_FLAG_ACTION_LEAVE,
        .subtract_flag = GB_FLAG_ACTION_UNSET,
        .half_carry_flag = GB_FLAG_ACTION_ACCORDINGLY,
        .carry_flag = GB_FLAG_ACTION_ACCORDINGLY,
    };

    instructions[0x80] = 
    (gb_instruction)
    {
        .operation = GB_OPERATION_ADD,
        .cycles = 4,
        .destination = 
        {
            .type = GB_OPERAND_REGISTER,
            .value8 = REG8INDEX(gb_register, A),
        },
        .source = 
        {
            .type = GB_OPERAND_REGISTER,
            .value8 = REG8INDEX(gb_register, B),
        },
        .zero_flag = GB_FLAG_ACTION_ACCORDINGLY,
        .subtract_flag = GB_FLAG_ACTION_UNSET,
        .half_carry_flag = GB_FLAG_ACTION_ACCORDINGLY,
        .carry_flag = GB_FLAG_ACTION_ACCORDINGLY,
    };

    instructions[0x81] = 
    (gb_instruction)
    {
        .operation = GB_OPERATION_ADD,
        .cycles = 4,
        .destination = 
        {
            .type = GB_OPERAND_REGISTER,
            .value8 = REG8INDEX(gb_register, A),
        },
        .source = 
        {
            .type = GB_OPERAND_REGISTER,
            .value8 = REG8INDEX(gb_register, C),
        }
    };

    instructions[0x82] = 
    (gb_instruction)
    {
        .operation = GB_OPERATION_ADD,
        .cycles = 4,
        .destination = 
        {
            .type = GB_OPERAND_REGISTER,
            .value8 = REG8INDEX(gb_register, A),
        },
        .source = 
        {
            .type = GB_OPERAND_REGISTER,
            .value8 = REG8INDEX(gb_register, D),
        },
        .zero_flag = GB_FLAG_ACTION_ACCORDINGLY,
        .subtract_flag = GB_FLAG_ACTION_UNSET,
        .half_carry_flag = GB_FLAG_ACTION_ACCORDINGLY,
        .carry_flag = GB_FLAG_ACTION_ACCORDINGLY,
    };

    instructions[0x83] = 
    (gb_instruction)
    {
        .operation = GB_OPERATION_ADD,
        .cycles = 4,
        .destination = 
        {
            .type = GB_OPERAND_REGISTER,
            .value8 = REG8INDEX(gb_register, A),
        },
        .source = 
        {
            .type = GB_OPERAND_REGISTER,
            .value8 = REG8INDEX(gb_register, E),
        },
        .zero_flag = GB_FLAG_ACTION_ACCORDINGLY,
        .subtract_flag = GB_FLAG_ACTION_UNSET,
        .half_carry_flag = GB_FLAG_ACTION_ACCORDINGLY,
        .carry_flag = GB_FLAG_ACTION_ACCORDINGLY,
    };

    instructions[0x84] = 
    (gb_instruction)
    {
        .operation = GB_OPERATION_ADD,
        .cycles = 4,
        .destination = 
        {
            .type = GB_OPERAND_REGISTER,
            .value8 = REG8INDEX(gb_register, A),
        },
        .source = 
        {
            .type = GB_OPERAND_REGISTER,
            .value8 = REG8INDEX(gb_register, H),
        },
        .zero_flag = GB_FLAG_ACTION_ACCORDINGLY,
        .subtract_flag = GB_FLAG_ACTION_UNSET,
        .half_carry_flag = GB_FLAG_ACTION_ACCORDINGLY,
        .carry_flag = GB_FLAG_ACTION_ACCORDINGLY,
    };

    instructions[0x85] = 
    (gb_instruction)
    {
        .operation = GB_OPERATION_ADD,
        .cycles = 4,
        .destination = 
        {
            .type = GB_OPERAND_REGISTER,
            .value8 = REG8INDEX(gb_register, A),
        },
        .source = 
        {
            .type = GB_OPERAND_REGISTER,
            .value8 = REG8INDEX(gb_register, L),
        },
        .zero_flag = GB_FLAG_ACTION_ACCORDINGLY,
        .subtract_flag = GB_FLAG_ACTION_UNSET,
        .half_carry_flag = GB_FLAG_ACTION_ACCORDINGLY,
        .carry_flag = GB_FLAG_ACTION_ACCORDINGLY,
    };

    instructions[0x86] = 
    (gb_instruction)
    {
        .operation = GB_OPERATION_ADD,
        .cycles = 8,
        .destination = 
        {
            .type = GB_OPERAND_REGISTER,
            .value8 = REG8INDEX(gb_register, A),
        },
        .source = 
        {
            .type = GB_OPERAND_REGISTER_ADDRESS,
            .value16 = REG16INDEX(gb_register, HL),
            .wide = true,
        },
        .zero_flag = GB_FLAG_ACTION_ACCORDINGLY,
        .subtract_flag = GB_FLAG_ACTION_UNSET,
        .half_carry_flag = GB_FLAG_ACTION_ACCORDINGLY,
        .carry_flag = GB_FLAG_ACTION_ACCORDINGLY,
    };

    instructions[0x87] = 
    (gb_instruction)
    {
        .operation = GB_OPERATION_ADD,
        .cycles = 4,
        .destination = 
        {
            .type = GB_OPERAND_REGISTER,
            .value8 = REG8INDEX(gb_register, A),
        },
        .source = 
        {
            .type = GB_OPERAND_REGISTER_ADDRESS,
            .value8 = REG8INDEX(gb_register, A),
        },
        .zero_flag = GB_FLAG_ACTION_ACCORDINGLY,
        .subtract_flag = GB_FLAG_ACTION_UNSET,
        .half_carry_flag = GB_FLAG_ACTION_ACCORDINGLY,
        .carry_flag = GB_FLAG_ACTION_ACCORDINGLY,
    };

    instructions[0xC6] = 
    (gb_instruction)
    {
        .operation = GB_OPERATION_ADD,
        .additional_bytes = 1,
        .cycles = 8,
        .destination = 
        {
            .type = GB_OPERAND_REGISTER,
            .value8 = REG8INDEX(gb_register, A),
        },
        .source = 
        {
            .type = GB_OPERAND_IMMEDIATE,
        },
        .zero_flag = GB_FLAG_ACTION_ACCORDINGLY,
        .subtract_flag = GB_FLAG_ACTION_UNSET,
        .half_carry_flag = GB_FLAG_ACTION_ACCORDINGLY,
        .carry_flag = GB_FLAG_ACTION_ACCORDINGLY,
    };

    instructions[0xE8] = 
    (gb_instruction)
    {
        .operation = GB_OPERATION_ADD,
        .additional_bytes = 1,
        .cycles = 16,
        .destination = 
        {
            .type = GB_OPERAND_REGISTER,
            .value16 = REG16INDEX(gb_register, SP),
        },
        .source = 
        {
            .type = GB_OPERAND_IMMEDIATE,
        },
        .zero_flag = GB_FLAG_ACTION_UNSET,
        .subtract_flag = GB_FLAG_ACTION_UNSET,
        .half_carry_flag = GB_FLAG_ACTION_ACCORDINGLY,
        .carry_flag = GB_FLAG_ACTION_ACCORDINGLY,
    };
}

u16
get_operand_value(gb_state *state, gb_operand operand)
{
    u16 value = 0;
    if(operand.wide)
    {
        if(operand.type == GB_OPERAND_REGISTER)
        {
            value = state->reg.registers_wide[operand.value16];
        }
        else if(operand.type == GB_OPERAND_IMMEDIATE)
        {
            value = operand.value16;
        }
        else if(operand.type == GB_OPERAND_REGISTER_ADDRESS)
        {
            u16 address = state->reg.registers_wide[operand.value16];
            value = *(u16*)&state->ram.bytes[address];
        }
    }
    else
    {
        if(operand.type == GB_OPERAND_REGISTER)
        {
            value = state->reg.registers[operand.value8];
        }
        else if(operand.type == GB_OPERAND_IMMEDIATE)
        {
            value = operand.value8;
        }
        else if(operand.type == GB_OPERAND_REGISTER_ADDRESS)
        {
            u8 address = state->reg.registers[operand.value8];
            value = state->ram.bytes[address];
        }
    }
    return(value);
}

void
set_destination_value(gb_state *state, gb_operand destination, u16 value)
{
    switch(destination.type)
    {
        case GB_OPERAND_REGISTER:
        {
            if(destination.wide)
            {
                state->reg.registers_wide[destination.value16] = value;
            }
            else
            {
                state->reg.registers[destination.value8] = value;
            }
        }
        break;
        case GB_OPERAND_REGISTER_ADDRESS:
        {
            u16 address = 0;
            if(destination.wide)
            {
                address = state->reg.registers_wide[destination.value16];
            }
            else
            {
                address = state->reg.registers[destination.value8];
            }
            state->ram.bytes[address] = value & 0xFF;
            if(destination.wide)
            {
                state->ram.bytes[address+1] = (value >> 8) & 0xFF;
            }
        }
        break;
        case GB_OPERAND_ADDRESS:
        {
            u16 address = destination.value16;
            state->ram.bytes[address] = value & 0xFF;
            if(destination.wide)
            {
                state->ram.bytes[address+1] = (value >> 8) & 0xFF;
            }
        }
        break;
        default: break;
    }
}

void 
gb_perform_instruction(gb_state *state)
{
    //TODO: this will not work for conditional cycle counts
    u64 delta_cycles = state->cycle - state->last_operation_cycle;
    gb_instruction instruction = state->current_instruction;
    gb_operand destination = instruction.destination;
    gb_operand source = instruction.source;
    gb_register *reg = &state->reg;
    gb_memory *ram = &state->ram;

    //TODO: state-> should not be used below here

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
                u16 dest_value = get_operand_value(state, destination);
                u16 source_value = get_operand_value(state, source);

                u16 result = dest_value + source_value;

                set_destination_value(state, destination, result);

                if(instruction.zero_flag == GB_FLAG_ACTION_ACCORDINGLY)
                {
                    if(destination.wide && result == 0)
                    {
                        set_zero_flag(reg);
                    }
                    else if(0 == (u8)result)
                    {
                        set_zero_flag(reg);
                    }
                    else
                    {
                        unset_zero_flag(reg);
                    }
                }

                if(instruction.half_carry_flag == GB_FLAG_ACTION_ACCORDINGLY)
                {
                    if(destination.wide)
                    {
                        if(((0xFF & source_value) + (0xFF & dest_value)) > 0xFF)
                        {
                            set_half_carry_flag(reg);
                        }
                        else
                        {
                            unset_half_carry_flag(reg);
                        }
                    }
                    else
                    {
                        if(((0xF & source_value) + (0xF & dest_value)) > 0xF)
                        {
                            set_half_carry_flag(reg);
                        }
                        else
                        {
                            unset_half_carry_flag(reg);
                        }
                    }
                }

                if(instruction.carry_flag == GB_FLAG_ACTION_ACCORDINGLY)
                {
                    if(destination.wide)
                    {
                        u32 result32 = (u32)source_value + (u32)dest_value;
                        if(result32 > 0xFFFF)
                        {
                            set_carry_flag(reg);
                        }
                        else
                        {
                            unset_carry_flag(reg);
                        }
                    }
                    else
                    {
                        if(result > 0xFF)
                        {
                            set_carry_flag(reg);
                        }
                        else
                        {
                            unset_carry_flag(reg);
                        }
                    }
                }

            }
            break;
            default:
            {
            }
            break;
        }
            
        //TODO: maybe these flags can be an array which can be looped through?

        if(instruction.zero_flag == GB_FLAG_ACTION_UNSET)
        {
            reg->F = UNSET_BIT(reg->F, 7);
        }
        else if(instruction.zero_flag == GB_FLAG_ACTION_SET)
        {
            reg->F = SET_BIT(reg->F, 7);
        }

        if(instruction.subtract_flag == GB_FLAG_ACTION_UNSET)
        {
            reg->F = UNSET_BIT(reg->F, 6);
        }
        else if(instruction.subtract_flag == GB_FLAG_ACTION_SET)
        {
            reg->F = SET_BIT(reg->F, 6);
        }

        if(instruction.half_carry_flag == GB_FLAG_ACTION_UNSET)
        {
            reg->F = UNSET_BIT(reg->F, 5);
        }
        else if(instruction.half_carry_flag == GB_FLAG_ACTION_SET)
        {
            reg->F = SET_BIT(reg->F, 5);
        }

        if(instruction.carry_flag == GB_FLAG_ACTION_UNSET)
        {
            reg->F = UNSET_BIT(reg->F, 4);
        }
        else if(instruction.carry_flag == GB_FLAG_ACTION_SET)
        {
            reg->F = SET_BIT(reg->F, 4);
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
read_sourceytes(gb_state *state, gb_operand *operand)
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
            if(operand_needs_more_bytes(instruction.destination))
            {
                read_sourceytes(state, &instruction.destination);
            }
            if(operand_needs_more_bytes(instruction.source))
            {
                read_sourceytes(state, &instruction.source);
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

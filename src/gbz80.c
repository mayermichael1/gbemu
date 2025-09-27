#include "include/gbz80.h"

#include "include/general.h"

#define REG16INDEX(registers, reg) (OFFSET_OF(registers, reg) / 2)

#define REG8INDEX(registers, reg) (OFFSET_OF(registers, reg))

#define REG16(reg_name) \
{ \
    .type = GB_OPERAND_REGISTER, \
    .wide = true, \
    .value16 = REG16INDEX(gb_register, reg_name), \
}

#define REG8(reg_name) \
{ \
    .type = GB_OPERAND_REGISTER, \
    .value8 = REG8INDEX(gb_register, reg_name), \
}

#define REG8ADDRESS(reg_name) \
{ \
    .type = GB_OPERAND_REGISTER_ADDRESS, \
    .value8 = REG8INDEX(gb_register, reg_name), \
}

#define REG16ADDRESS(reg_name) \
{ \
    .type = GB_OPERAND_REGISTER_ADDRESS, \
    .wide = true, \
    .value16 = REG16INDEX(gb_register, reg_name), \
}

#define IMMEDIATE8() \
{ \
    .type = GB_OPERAND_IMMEDIATE, \
}

#define IMMEDIATE16() \
{ \
    .type = GB_OPERAND_IMMEDIATE, \
    .wide = true, \
}

#define ADDRESS8() \
{ \
    .type = GB_OPERAND_ADDRESS, \
}

#define ADDRESS16() \
{ \
    .type = GB_OPERAND_ADDRESS, \
    .wide = true, \
}

#define ADD_HL_R16(reg_name) \
(gb_instruction) \
{ \
    .operation = GB_OPERATION_ADD, \
    .cycles = 8, \
    .destination = REG16(HL), \
    .source = REG16(reg_name), \
    .flag_actions[GB_FLAG_ZERO] = GB_FLAG_ACTION_LEAVE, \
    .flag_actions[GB_FLAG_SUBTRACTION] = GB_FLAG_ACTION_UNSET, \
    .flag_actions[GB_FLAG_HALF_CARRY] = GB_FLAG_ACTION_ACCORDINGLY, \
    .flag_actions[GB_FLAG_CARRY] = GB_FLAG_ACTION_ACCORDINGLY, \
}

#define ADD_A_R8(reg_name) \
(gb_instruction) \
{ \
    .operation = GB_OPERATION_ADD, \
    .cycles = 4, \
    .destination = REG8(A), \
    .source = REG8(reg_name), \
    .flag_actions[GB_FLAG_ZERO] = GB_FLAG_ACTION_ACCORDINGLY, \
    .flag_actions[GB_FLAG_SUBTRACTION] = GB_FLAG_ACTION_UNSET, \
    .flag_actions[GB_FLAG_HALF_CARRY] = GB_FLAG_ACTION_ACCORDINGLY, \
    .flag_actions[GB_FLAG_CARRY] = GB_FLAG_ACTION_ACCORDINGLY, \
}

#define LOAD_R16_IMMEDIATE(reg_name) \
(gb_instruction) \
{ \
    .operation = GB_OPERATION_LOAD, \
    .destination = REG16(reg_name), \
    .source = IMMEDIATE16(), \
    .cycles = 12, \
}

#define LOAD_R16ADDRESS_A(reg_name) \
(gb_instruction) \
{ \
    .operation = GB_OPERATION_LOAD, \
    .destination = REG16ADDRESS(reg_name), \
    .source = REG8(A), \
    .cycles = 8, \
}

#define LOAD_INCREMENT_R16ADDRESS_A(reg_name) \
(gb_instruction) \
{ \
    .operation = GB_OPERATION_LOAD_INCREMENT, \
    .destination = REG16ADDRESS(reg_name), \
    .source = REG8(A), \
    .cycles = 8, \
}

#define LOAD_DECREMENT_R16ADDRESS_A(reg_name) \
(gb_instruction) \
{ \
    .operation = GB_OPERATION_LOAD_DECREMENT, \
    .destination = REG16ADDRESS(reg_name), \
    .source = REG8(A), \
    .cycles = 8, \
}

#define LOAD_R8_IMMEDIATE(reg_name) \
(gb_instruction) \
{ \
    .operation = GB_OPERATION_LOAD, \
    .destination = REG8(reg_name), \
    .source = IMMEDIATE8(), \
    .cycles = 8, \
}

#define LOAD_HLADDRESS_REG8(reg_name) \
(gb_instruction) \
{ \
    .operation = GB_OPERATION_LOAD, \
    .destination = REG16ADDRESS(HL), \
    .source = REG8(reg_name), \
    .cycles = 8, \
}

#define LOAD_REG8_HLADDRESS(reg_name) \
(gb_instruction) \
{ \
    .operation = GB_OPERATION_LOAD, \
    .source = REG16ADDRESS(HL), \
    .destination = REG8(reg_name), \
    .cycles = 8, \
}

#define LOAD_R8_R8(dest_reg, source_reg) \
(gb_instruction) \
{ \
    .operation = GB_OPERATION_LOAD, \
    .destination = REG8(dest_reg), \
    .source = REG8(source_reg), \
    .cycles = 4, \
}

#define LOAD_A_R16ADDRESS(reg_name) \
(gb_instruction) \
{ \
    .operation = GB_OPERATION_LOAD, \
    .destination = REG8(A), \
    .source = REG16ADDRESS(reg_name), \
    .cycles = 8, \
}

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

    /// LOAD instructions
    instructions[0x01] = LOAD_R16_IMMEDIATE(BC);
    instructions[0x11] = LOAD_R16_IMMEDIATE(DE);
    instructions[0x21] = LOAD_R16_IMMEDIATE(HL);
    instructions[0x31] = LOAD_R16_IMMEDIATE(SP);

    instructions[0x02] = LOAD_R16ADDRESS_A(BC);
    instructions[0x12] = LOAD_R16ADDRESS_A(DE);
    instructions[0x22] = LOAD_INCREMENT_R16ADDRESS_A(HL);
    instructions[0x32] = LOAD_DECREMENT_R16ADDRESS_A(HL);

    instructions[0x06] = LOAD_R8_IMMEDIATE(B);
    instructions[0x16] = LOAD_R8_IMMEDIATE(D);
    instructions[0x26] = LOAD_R8_IMMEDIATE(H);
    instructions[0x36] = 
    (gb_instruction) 
    { 
        .operation = GB_OPERATION_LOAD, 
        .destination = REG16ADDRESS(HL),
        .source = IMMEDIATE8(),
        .cycles = 12,
    };
    instructions[0x08] = 
    (gb_instruction) 
    { 
        .operation = GB_OPERATION_LOAD, 
        .destination = ADDRESS16(),
        .source = REG16(SP),
        .cycles = 20,
    };

    instructions[0x0A] = LOAD_A_R16ADDRESS(BC);
    instructions[0x1A] = LOAD_A_R16ADDRESS(DE);
    instructions[0x2A] =
    (gb_instruction) 
    { 
        .operation = GB_OPERATION_LOAD_INCREMENT, 
        .destination = REG8(A),
        .source = REG16ADDRESS(HL),
        .cycles = 8,
    };
    instructions[0x3A] = 
    (gb_instruction) 
    { 
        .operation = GB_OPERATION_LOAD_DECREMENT,
        .destination = REG8(A),
        .source = REG16ADDRESS(HL),
        .cycles = 8,
    };

    instructions[0x0E] = LOAD_R8_IMMEDIATE(C);
    instructions[0x1E] = LOAD_R8_IMMEDIATE(E);
    instructions[0x2E] = LOAD_R8_IMMEDIATE(L);
    instructions[0x3E] = LOAD_R8_IMMEDIATE(A);

    instructions[0x40] = LOAD_R8_R8(B, B);
    instructions[0x50] = LOAD_R8_R8(D, B);
    instructions[0x60] = LOAD_R8_R8(H, B);
    instructions[0x70] = LOAD_HLADDRESS_REG8(B);

    instructions[0x41] = LOAD_R8_R8(B, C);
    instructions[0x51] = LOAD_R8_R8(D, C);
    instructions[0x61] = LOAD_R8_R8(H, C);
    instructions[0x71] = LOAD_HLADDRESS_REG8(C);

    instructions[0x42] = LOAD_R8_R8(B, D);
    instructions[0x52] = LOAD_R8_R8(D, D); 
    instructions[0x62] = LOAD_R8_R8(H, D); 
    instructions[0x71] = LOAD_HLADDRESS_REG8(D);

    instructions[0x43] = LOAD_R8_R8(B, E);
    instructions[0x53] = LOAD_R8_R8(D, E); 
    instructions[0x63] = LOAD_R8_R8(H, E); 
    instructions[0x73] = LOAD_HLADDRESS_REG8(E);

    instructions[0x44] = LOAD_R8_R8(B, H);
    instructions[0x54] = LOAD_R8_R8(D, H); 
    instructions[0x64] = LOAD_R8_R8(H, H); 
    instructions[0x74] = LOAD_HLADDRESS_REG8(H);

    instructions[0x45] = LOAD_R8_R8(B, L);
    instructions[0x55] = LOAD_R8_R8(D, L); 
    instructions[0x65] = LOAD_R8_R8(H, L); 
    instructions[0x75] = LOAD_HLADDRESS_REG8(L);

    instructions[0x46] = LOAD_REG8_HLADDRESS(B);
    instructions[0x56] = LOAD_REG8_HLADDRESS(D); 
    instructions[0x66] = LOAD_REG8_HLADDRESS(H); 
    
    instructions[0x47] = LOAD_R8_R8(B, A);
    instructions[0x57] = LOAD_R8_R8(D, A);
    instructions[0x67] = LOAD_R8_R8(H, A);
    instructions[0x77] = LOAD_HLADDRESS_REG8(A);

    instructions[0x48] = LOAD_R8_R8(C, B);
    instructions[0x58] = LOAD_R8_R8(E, B);
    instructions[0x68] = LOAD_R8_R8(L, B);
    instructions[0x78] = LOAD_R8_R8(A, B);

    instructions[0x49] = LOAD_R8_R8(C, C);
    instructions[0x59] = LOAD_R8_R8(E, C);
    instructions[0x69] = LOAD_R8_R8(L, C);
    instructions[0x79] = LOAD_R8_R8(A, C);

    instructions[0x4A] = LOAD_R8_R8(C, D);
    instructions[0x5A] = LOAD_R8_R8(E, D);
    instructions[0x6A] = LOAD_R8_R8(L, D);
    instructions[0x7A] = LOAD_R8_R8(A, D);

    instructions[0x4B] = LOAD_R8_R8(C, E);
    instructions[0x5B] = LOAD_R8_R8(E, E);
    instructions[0x6B] = LOAD_R8_R8(L, E);
    instructions[0x7B] = LOAD_R8_R8(A, E);

    instructions[0x4C] = LOAD_R8_R8(C, H);
    instructions[0x5C] = LOAD_R8_R8(E, H);
    instructions[0x6C] = LOAD_R8_R8(L, H);
    instructions[0x7C] = LOAD_R8_R8(A, H);

    instructions[0x4D] = LOAD_R8_R8(C, L);
    instructions[0x5D] = LOAD_R8_R8(E, L);
    instructions[0x6D] = LOAD_R8_R8(L, L);
    instructions[0x7D] = LOAD_R8_R8(A, L);

    instructions[0x4E] = LOAD_REG8_HLADDRESS(C);
    instructions[0x5E] = LOAD_REG8_HLADDRESS(E); 
    instructions[0x6E] = LOAD_REG8_HLADDRESS(L); 
    instructions[0x7E] = LOAD_REG8_HLADDRESS(A); 

    instructions[0x4F] = LOAD_R8_R8(C, A);
    instructions[0x5F] = LOAD_R8_R8(E, A);
    instructions[0x6F] = LOAD_R8_R8(L, A);
    instructions[0x7F] = LOAD_R8_R8(A, A);

    // missing 0xE0 
    instructions[0xE0] = 
    (gb_instruction)
    {
        .operation = GB_OPERATION_LOAD_HIGH,
        .cycles = 12,
        .destination = ADDRESS8(),
        .source = REG8(A),
    };
    instructions[0xF0] = 
    (gb_instruction)
    {
        .operation = GB_OPERATION_LOAD_HIGH,
        .cycles = 12,
        .destination = REG8(A),
        .source = ADDRESS8(),
    };

    instructions[0xE2] = 
    (gb_instruction)
    {
        .operation = GB_OPERATION_LOAD_HIGH,
        .cycles = 8,
        .destination = REG8ADDRESS(C),
        .source = REG8(A),
    };
    instructions[0xF2] = 
    (gb_instruction)
    {
        .operation = GB_OPERATION_LOAD_HIGH,
        .cycles = 8,
        .source = REG8ADDRESS(C),
        .destination = REG8(A),
    };
    
    // missing 0xF8
    // missing 0xF9

    // missing 0xEA
    // missing 0xFA

    /// ADD instructions

    instructions[0x09] = ADD_HL_R16(BC);
    instructions[0x19] = ADD_HL_R16(DE);
    instructions[0x29] = ADD_HL_R16(HL);
    instructions[0x39] = ADD_HL_R16(SP);

    instructions[0x80] = ADD_A_R8(B);
    instructions[0x81] = ADD_A_R8(C);
    instructions[0x82] = ADD_A_R8(D);
    instructions[0x83] = ADD_A_R8(E);
    instructions[0x84] = ADD_A_R8(H);
    instructions[0x85] = ADD_A_R8(L);
    instructions[0x87] = ADD_A_R8(A);

    instructions[0x86] = 
    (gb_instruction)
    {
        .operation = GB_OPERATION_ADD,
        .cycles = 8,
        .destination = REG8(A),
        .source = REG16ADDRESS(HL),
        .flag_actions[GB_FLAG_ZERO] = GB_FLAG_ACTION_ACCORDINGLY, 
        .flag_actions[GB_FLAG_SUBTRACTION] = GB_FLAG_ACTION_UNSET, 
        .flag_actions[GB_FLAG_HALF_CARRY] = GB_FLAG_ACTION_ACCORDINGLY, 
        .flag_actions[GB_FLAG_CARRY] = GB_FLAG_ACTION_ACCORDINGLY, 
    };

    instructions[0xC6] = 
    (gb_instruction)
    {
        .operation = GB_OPERATION_ADD,
        .cycles = 8,
        .destination = REG8(A),
        .source = IMMEDIATE8(),
        .flag_actions[GB_FLAG_ZERO] = GB_FLAG_ACTION_ACCORDINGLY, 
        .flag_actions[GB_FLAG_SUBTRACTION] = GB_FLAG_ACTION_UNSET, 
        .flag_actions[GB_FLAG_HALF_CARRY] = GB_FLAG_ACTION_ACCORDINGLY, 
        .flag_actions[GB_FLAG_CARRY] = GB_FLAG_ACTION_ACCORDINGLY, 
    };

    instructions[0xE8] = 
    (gb_instruction)
    {
        .operation = GB_OPERATION_ADD,
        .cycles = 16,
        .destination = REG16(SP),
        .source = IMMEDIATE8(),
        .flag_actions[GB_FLAG_ZERO] = GB_FLAG_ACTION_UNSET, 
        .flag_actions[GB_FLAG_SUBTRACTION] = GB_FLAG_ACTION_UNSET, 
        .flag_actions[GB_FLAG_HALF_CARRY] = GB_FLAG_ACTION_ACCORDINGLY, 
        .flag_actions[GB_FLAG_CARRY] = GB_FLAG_ACTION_ACCORDINGLY, 
    };
}

u16
get_operand_value_offset(gb_state *state, gb_operand operand, u16 offset)
{
    u16 value = 0;
    if(operand.wide)
    {
        switch(operand.type)
        {
            case GB_OPERAND_REGISTER:
            {
                value = state->reg.registers_wide[operand.value16];
            }
            break;
            case GB_OPERAND_IMMEDIATE:
            {
                value = operand.value16;
            }
            break;
            case GB_OPERAND_ADDRESS:
            {
                u16 address = operand.value16; 
                address += offset;
                value = state->ram.bytes[address];
            }
            break;
            case GB_OPERAND_REGISTER_ADDRESS:
            {
                u16 address = state->reg.registers_wide[operand.value16];
                address += offset;
                value = state->ram.bytes[address];
            }
            break;
            default:
            {
                ASSERT(false);
            }
            break;
        }
    }
    else
    {
        switch(operand.type)
        {
            case GB_OPERAND_REGISTER:
            {
                value = state->reg.registers[operand.value8];
            }
            break;
            case GB_OPERAND_IMMEDIATE:
            {
                value = operand.value8;
                value += offset;
            }
            break;
            case GB_OPERAND_ADDRESS:
            {
                u16 address = operand.value8; 
                address += offset;
                value = state->ram.bytes[address];
            }
            break;
            default:
            {
                ASSERT(false);
            }
            break;
        }
    }
    return(value);
}

u16
get_operand_value(gb_state *state, gb_operand operand)
{
    return get_operand_value_offset(state, operand, 0);
}

void 
set_value_offset(gb_state *state, gb_operand destination, u16 value, u16 offset)
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
            address += offset;
            state->ram.bytes[address] = value & 0xFF;
        }
        break;
        case GB_OPERAND_ADDRESS:
        {
            u16 address = destination.value16;
            address += offset;
            state->ram.bytes[address] = value & 0xFF;
        }
        break;
        default: break;
    }
}
void
set_value(gb_state *state, gb_operand destination, u16 value)
{
    set_value_offset(state, destination, value, 0);
}

b8
is_operand_address(gb_operand operand)
{
    b8 is_address = false;
    if( operand.type == GB_OPERAND_REGISTER_ADDRESS ||
        operand.type == GB_OPERAND_ADDRESS)
    {
        is_address = true;
    }
    return(is_address);
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
                u16 destination_value = get_operand_value(state, destination);
                u16 source_value = get_operand_value(state, source);

                u16 result = destination_value + source_value;

                set_value(state, destination, result);

                if(instruction.flag_actions[GB_FLAG_ZERO] == GB_FLAG_ACTION_ACCORDINGLY)
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

                if(instruction.flag_actions[GB_FLAG_HALF_CARRY] == GB_FLAG_ACTION_ACCORDINGLY)
                {
                    if(destination.wide)
                    {
                        if(((0xFF & source_value) + (0xFF & destination_value)) > 0xFF)
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
                        if(((0xF & source_value) + (0xF & destination_value)) > 0xF)
                        {
                            set_half_carry_flag(reg);
                        }
                        else
                        {
                            unset_half_carry_flag(reg);
                        }
                    }
                }

                if(instruction.flag_actions[GB_FLAG_CARRY] == GB_FLAG_ACTION_ACCORDINGLY)
                {
                    if(destination.wide)
                    {
                        u32 result32 = (u32)source_value + (u32)destination_value;
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
            case GB_OPERATION_LOAD:
            {
                u16 source_value = get_operand_value(state, source);
                set_value(state, destination, source_value);
            }
            break;
            case GB_OPERATION_LOAD_INCREMENT:
            {
                u16 source_value = get_operand_value(state, source);
                set_value(state, destination, source_value);

                if(destination.type == GB_OPERAND_REGISTER_ADDRESS)
                {
                    ASSERT(destination.wide);
                    reg->registers_wide[destination.value16]++;
                }
                else if(source.type == GB_OPERAND_REGISTER_ADDRESS)
                {
                    ASSERT(source.wide);
                    reg->registers_wide[source.value16]++;
                }
                else
                {
                    ASSERT(false);
                }
            }
            break;
            case GB_OPERATION_LOAD_DECREMENT:
            {
                u16 source_value = get_operand_value(state, source);
                set_value(state, destination, source_value);
                if(destination.type == GB_OPERAND_REGISTER_ADDRESS)
                {
                    ASSERT(destination.wide);
                    reg->registers_wide[destination.value16]--;
                }
                else if(source.type == GB_OPERAND_REGISTER_ADDRESS)
                {
                    ASSERT(source.wide);
                    reg->registers_wide[source.value16]--;
                }
                else
                {
                    ASSERT(false);
                }
            }
            break;
            case GB_OPERATION_LOAD_HIGH: 
            {
                u16 source_value = get_operand_value(state, source);
                if(is_operand_address(source))
                {
                    source_value = get_operand_value_offset(state, source, 0xFF00);
                }
                if(is_operand_address(destination))
                {
                    set_value_offset(state, destination, source_value, 0xFF00);
                }
                else
                {
                    set_value(state, destination, source_value);
                }
            }
            break;
            break;
            default:
            {
                ASSERT(1);
            }
            break;
        }
            
        for(u8 i = 0; i < 8; ++i)
        {
            if(instruction.flag_actions[i] == GB_FLAG_ACTION_UNSET)
            {
                reg->F = UNSET_BIT(reg->F, i);
            }
            else if(instruction.flag_actions[i] == GB_FLAG_ACTION_UNSET)
            {
                reg->F = SET_BIT(reg->F, i);
            }
        }

        state->current_instruction= (gb_instruction){0};
        state->last_operation_cycle = state->cycle;
    }
}

u8
operand_needs_more_bytes(gb_operand operand)
{
    u8 more_bytes = 0;
    if( operand.type == GB_OPERAND_ADDRESS || 
        operand.type == GB_OPERAND_IMMEDIATE)
    {
        if(operand.wide)
        {
            more_bytes = 2;
        }
        else
        {
            more_bytes = 1;
        }
    }
    return(more_bytes);
}

void
gb_load_next_instruction(gb_state *state)
{
    if(!state->current_instruction.operation)
    {
        u8 opcode = state->ram.bytes[state->reg.PC];
        gb_instruction instruction = instructions[opcode];

        // TODO: maybe this should be unified in some way
        u8 destination_bytes = operand_needs_more_bytes(instruction.destination);
        u8 source_bytes = operand_needs_more_bytes(instruction.source);

        // NOTE:    it will never happen, that source and destination need additional 
        //          bytes
        if(destination_bytes >= 1)
        {
            instruction.destination.value8 = state->ram.bytes[state->reg.PC+1];
        }
        if(destination_bytes == 2)
        {
            instruction.destination.value8_high = state->ram.bytes[state->reg.PC+2];
        }

        if(source_bytes >= 1)
        {
            instruction.source.value8 = state->ram.bytes[state->reg.PC+1];
        }
        if(source_bytes == 2)
        {
            instruction.source.value8_high = state->ram.bytes[state->reg.PC+2];
        }

        //TODO: should PC be increased here already? 
        state->reg.PC += 1 + destination_bytes + source_bytes;
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

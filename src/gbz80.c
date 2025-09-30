#include "include/gbz80.h"

#include "include/general.h"

#define REG16_INDEX(reg) (OFFSET_OF(gb_register, reg) / 2)

#define REG16(reg_name) \
{ \
    .type = GB_OPERAND_REGISTER, \
    .operand_wide = true, \
    .value_wide = true, \
    .value16 = REG16_INDEX(reg_name), \
}

#define REG16ADDRESS(reg_name) \
{ \
    .type = GB_OPERAND_REGISTER_ADDRESS, \
    .value16 = REG16_INDEX(reg_name), \
    .operand_wide = true, \
}



#define REG8_INDEX(reg) (OFFSET_OF(gb_register, reg))

#define REG8(reg_name) \
{ \
    .type = GB_OPERAND_REGISTER, \
    .value8 = REG8_INDEX(reg_name), \
}

#define REG8ADDRESS(reg_name) \
{ \
    .type = GB_OPERAND_REGISTER_ADDRESS, \
    .value8 = REG8_INDEX(reg_name), \
}



#define IMM8() \
{ \
    .type = GB_OPERAND_IMMEDIATE, \
}

#define IMM16() \
{ \
    .type = GB_OPERAND_IMMEDIATE, \
    .value_wide = true, \
    .operand_wide = true, \
}

#define ADR8() \
{ \
    .type = GB_OPERAND_ADDRESS, \
}

#define ADR16() \
{ \
    .type = GB_OPERAND_ADDRESS, \
    .operand_wide = true, \
}



#define ADD_HL_REG16(reg_name) \
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

#define ADD_HL_R8(reg_name) \
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

#define LOAD_REG16_IMM16(reg_name) \
(gb_instruction) \
{ \
    .operation = GB_OPERATION_LOAD, \
    .destination = REG16(reg_name), \
    .source = IMM16(), \
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

#define LOAD_R8_IMM8(reg_name) \
(gb_instruction) \
{ \
    .operation = GB_OPERATION_LOAD, \
    .destination = REG8(reg_name), \
    .source = IMM8(), \
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



#define INC_REG16(reg_name) \
(gb_instruction) \
{ \
    .operation = GB_OPERATION_INC, \
    .cycles = 8, \
    .destination = REG16(reg_name), \
}

#define INC_REG8(reg_name) \
(gb_instruction) \
{ \
    .operation = GB_OPERATION_INC, \
    .cycles = 4, \
    .destination = REG8(reg_name), \
    .flag_actions[GB_FLAG_ZERO] = GB_FLAG_ACTION_ACCORDINGLY, \
    .flag_actions[GB_FLAG_SUBTRACTION] = GB_FLAG_ACTION_UNSET, \
    .flag_actions[GB_FLAG_HALF_CARRY] = GB_FLAG_ACTION_ACCORDINGLY, \
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
set_or_unset_zero_flag(gb_register *reg, b8 set_bit)
{
    if(set_bit)
    {
        set_zero_flag(reg);
    }
    else
    {
        unset_zero_flag(reg);
    }
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
set_or_unset_half_carry_flag(gb_register *reg, b8 set_bit)
{    
    if(set_bit)
    {
        set_half_carry_flag(reg);
    }
    else
    {
        unset_half_carry_flag(reg);
    }

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
set_or_unset_carry_flag(gb_register *reg, b8 set_bit)
{
    if(set_bit)
    {
        set_carry_flag(reg);
    }
    else
    {
        unset_carry_flag(reg);
    }
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

    /// INC instructions
    
    instructions[0x03] = INC_REG16(BC);
    instructions[0x13] = INC_REG16(DE);
    instructions[0x23] = INC_REG16(HL);
    instructions[0x33] = INC_REG16(SP);

    instructions[0x04] = INC_REG8(B);
    instructions[0x14] = INC_REG8(D);
    instructions[0x24] = INC_REG8(H);

    instructions[0x34] = 
    (gb_instruction)
    {
        .operation = GB_OPERATION_INC,
        .cycles = 12,
        .destination = REG16ADDRESS(HL),
        .flag_actions[GB_FLAG_ZERO] = GB_FLAG_ACTION_ACCORDINGLY,
        .flag_actions[GB_FLAG_SUBTRACTION] = GB_FLAG_ACTION_UNSET,
        .flag_actions[GB_FLAG_HALF_CARRY] = GB_FLAG_ACTION_ACCORDINGLY,
    };

    instructions[0x0C] = INC_REG8(C);
    instructions[0x1C] = INC_REG8(E);
    instructions[0x2C] = INC_REG8(L);
    instructions[0x3C] = INC_REG8(A);


    /// LOAD instructions
    instructions[0x01] = LOAD_REG16_IMM16(BC);
    instructions[0x11] = LOAD_REG16_IMM16(DE);
    instructions[0x21] = LOAD_REG16_IMM16(HL);
    instructions[0x31] = LOAD_REG16_IMM16(SP);

    instructions[0x02] = LOAD_R16ADDRESS_A(BC);
    instructions[0x12] = LOAD_R16ADDRESS_A(DE);
    instructions[0x22] = LOAD_INCREMENT_R16ADDRESS_A(HL);
    instructions[0x32] = LOAD_DECREMENT_R16ADDRESS_A(HL);

    instructions[0x06] = LOAD_R8_IMM8(B);
    instructions[0x16] = LOAD_R8_IMM8(D);
    instructions[0x26] = LOAD_R8_IMM8(H);
    instructions[0x36] = 
    (gb_instruction) 
    { 
        .operation = GB_OPERATION_LOAD, 
        .destination = REG16ADDRESS(HL),
        .source = IMM8(),
        .cycles = 12,
    };
    instructions[0x08] = 
    (gb_instruction) 
    { 
        .operation = GB_OPERATION_LOAD, 
        .destination = ADR16(),
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

    instructions[0x0E] = LOAD_R8_IMM8(C);
    instructions[0x1E] = LOAD_R8_IMM8(E);
    instructions[0x2E] = LOAD_R8_IMM8(L);
    instructions[0x3E] = LOAD_R8_IMM8(A);

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

    instructions[0xE0] = 
    (gb_instruction)
    {
        .operation = GB_OPERATION_LOAD_HIGH,
        .cycles = 12,
        .destination = ADR8(),
        .source = REG8(A),
    };
    instructions[0xF0] = 
    (gb_instruction)
    {
        .operation = GB_OPERATION_LOAD_HIGH,
        .cycles = 12,
        .destination = REG8(A),
        .source = ADR8(),
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
        .destination = REG8(A),
        .source = REG8ADDRESS(C),
    };
    
    // missing 0xF8
    instructions[0xF8] =
    (gb_instruction)
    {
        .operation = GB_OPERATION_LOAD,
        .cycles = 12,
        .destination = REG16(HL),
        .source = 
        {
            .type = GB_OPERAND_REGISTER_OFFSET,
            .value16 = REG16_INDEX(SP),
            .operand_wide = true,
            .value_wide = true,
        },
        .flag_actions[GB_FLAG_HALF_CARRY] = GB_FLAG_ACTION_ACCORDINGLY,
        .flag_actions[GB_FLAG_CARRY] = GB_FLAG_ACTION_ACCORDINGLY,
    };

    instructions[0xF9] = 
    (gb_instruction)
    {
        .operation = GB_OPERATION_LOAD,
        .cycles = 8,
        .destination = REG16(SP),
        .source = REG16(HL),
    };

    instructions[0xEA] = 
    (gb_instruction)
    {
        .operation = GB_OPERATION_LOAD,
        .cycles = 16,
        .destination = ADR16(),
        .source = REG8(A),
    };
    instructions[0xFA] = 
    (gb_instruction)
    {
        .operation = GB_OPERATION_LOAD,
        .cycles = 16,
        .destination = REG8(A),
        .source = ADR16(),
    };

    /// ADD instructions

    instructions[0x09] = ADD_HL_REG16(BC);
    instructions[0x19] = ADD_HL_REG16(DE);
    instructions[0x29] = ADD_HL_REG16(HL);
    instructions[0x39] = ADD_HL_REG16(SP);

    instructions[0x80] = ADD_HL_R8(B);
    instructions[0x81] = ADD_HL_R8(C);
    instructions[0x82] = ADD_HL_R8(D);
    instructions[0x83] = ADD_HL_R8(E);
    instructions[0x84] = ADD_HL_R8(H);
    instructions[0x85] = ADD_HL_R8(L);
    instructions[0x87] = ADD_HL_R8(A);

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
        .source = IMM8(),
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
        .source = IMM8(),
        .flag_actions[GB_FLAG_ZERO] = GB_FLAG_ACTION_UNSET, 
        .flag_actions[GB_FLAG_SUBTRACTION] = GB_FLAG_ACTION_UNSET, 
        .flag_actions[GB_FLAG_HALF_CARRY] = GB_FLAG_ACTION_ACCORDINGLY, 
        .flag_actions[GB_FLAG_CARRY] = GB_FLAG_ACTION_ACCORDINGLY, 
    };
}

u16 
get_operand_address_offset(gb_state *state, gb_operand operand, u16 offset)
{
    u16 address = 0;
    switch(operand.type)
    {
        case GB_OPERAND_ADDRESS:
        {
            //NOTE: as upper 8 bits should be 0 on none wide operands this 
            //      should work
            address = operand.value16; 
            address += offset;
        }
        break;
        case GB_OPERAND_REGISTER_ADDRESS:
        {
            // NOTE: register addresses are always "wide"
            address = state->reg.registers_wide[operand.value16];
            address+=offset;
        }
        break;
        default:
        {
            address = 0xFFFF;
        }
        break;
    }
    return(address);
}

u16
get_operand_value_offset(gb_state *state, gb_operand operand, u16 offset)
{
    u16 value = 0;
    switch(operand.type)
    {
        case GB_OPERAND_REGISTER:
        case GB_OPERAND_REGISTER_OFFSET:
        {
            if(operand.operand_wide)
            {
                value = state->reg.registers_wide[operand.value16];
            }
            else
            {
                value = state->reg.registers[operand.value8];
            }

            value += operand.offset8;
        }
        break;
        case GB_OPERAND_IMMEDIATE:
        {
            //NOTE: the upper 8 bits are always 0 when the operation is not 
            //      wide, thus this should work without a problem
            value = operand.value16;
            value += offset;
        }
        break;
        case GB_OPERAND_ADDRESS:
        case GB_OPERAND_REGISTER_ADDRESS:
        {
            u16 address = get_operand_address_offset(state, operand, offset); 
            value = state->ram.bytes[address];
        }
        break;
        default:
        {
            ASSERT(false);
        }
        break;
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
            if(destination.operand_wide)
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
            if(destination.operand_wide)
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

b8
check_half_carry(u16 a, u16 b, b8 wide)
{
    b8 half_carry = false;
    if(wide)
    {
        if(((0xFF & a) + (0xFF & b)) > 0xFF)
        {
            half_carry = true;
        }
    }
    else
    {
        if(((0xF & a) + (0xF & b)) > 0xF)
        {
            half_carry = true;
        }
    }
    return(half_carry);
}

b8
check_carry(u16 a, u16 b, b8 wide)
{
    b8 carry = false;
    u32 result32 = (u32)a + (u32)b;
    if(wide)
    {
        if(result32 > 0xFFFF)
        {
            carry = true;
        }
    }
    else
    {
        if(result32 > 0xFF)
        {
            carry = true;
        }
    }
    return(carry);
}

b8
check_zero(u16 value, b8 wide)
{
    b8 zero = 0;
    if(wide)
    {
        if(value == 0)
        {
            zero = true;
        }
    }
    else
    {
        if((zero & 0xFF) == 0)
        {
            zero = true;
        }
    }
    return(zero);
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
                u16 destination_value = get_operand_value(state, destination);
                u16 source_value = get_operand_value(state, source);

                u16 result = destination_value + source_value;

                set_value(state, destination, result);

                if(instruction.flag_actions[GB_FLAG_ZERO] == GB_FLAG_ACTION_ACCORDINGLY)
                {
                    set_or_unset_zero_flag(reg, check_zero(result, destination.value_wide));
                }

                if(instruction.flag_actions[GB_FLAG_HALF_CARRY] == GB_FLAG_ACTION_ACCORDINGLY)
                {
                    set_or_unset_half_carry_flag(reg, check_half_carry(destination_value, source_value, destination.value_wide));
                }

                if(instruction.flag_actions[GB_FLAG_CARRY] == GB_FLAG_ACTION_ACCORDINGLY)
                {
                    set_or_unset_carry_flag(reg, check_carry(destination_value, source_value, destination.value_wide));
                }

            }
            break;
            case GB_OPERATION_LOAD:
            {
                u16 source_value = get_operand_value(state, source);
                u16 offset = source.offset8;
                u16 source_without_offset = source_value - source.offset8;
                if(source.type == GB_OPERAND_REGISTER_OFFSET)
                {
                    if(instruction.flag_actions[GB_FLAG_HALF_CARRY] == GB_FLAG_ACTION_ACCORDINGLY)
                    {
                        set_or_unset_half_carry_flag(reg, check_half_carry(source_without_offset, offset, source.value_wide));
                    }

                    if(instruction.flag_actions[GB_FLAG_CARRY] == GB_FLAG_ACTION_ACCORDINGLY)
                    {
                        set_or_unset_carry_flag(reg,check_carry(source_without_offset, offset, source.value_wide));
                    }

                }
                set_value(state, destination, source_value);
            }
            break;
            case GB_OPERATION_LOAD_INCREMENT:
            {
                u16 source_value = get_operand_value(state, source);
                set_value(state, destination, source_value);

                if(destination.type == GB_OPERAND_REGISTER_ADDRESS)
                {
                    reg->registers_wide[destination.value16]++;
                }
                else if(source.type == GB_OPERAND_REGISTER_ADDRESS)
                {
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
                    reg->registers_wide[destination.value16]--;
                }
                else if(source.type == GB_OPERAND_REGISTER_ADDRESS)
                {
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
            case GB_OPERATION_INC:
            {
                //TODO: carry flag is never set. Can INC never overflow?
                u16 source_value = get_operand_value(state, destination);
                source_value++;
                set_value(state, destination, source_value);

                if(instruction.flag_actions[GB_FLAG_ZERO] == GB_FLAG_ACTION_ACCORDINGLY)
                {
                    set_or_unset_zero_flag(reg, check_zero(source_value, source.value_wide));
                }

                if(instruction.flag_actions[GB_FLAG_HALF_CARRY] == GB_FLAG_ACTION_ACCORDINGLY)
                {
                    set_or_unset_half_carry_flag(reg, check_half_carry((source_value-1), 1, destination.value_wide));
                }

            }
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
        operand.type == GB_OPERAND_IMMEDIATE ||
        operand.type == GB_OPERAND_REGISTER_OFFSET)
    {
        if(operand.operand_wide)
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

        u8 destination_bytes = operand_needs_more_bytes(instruction.destination);
        u8 source_bytes = operand_needs_more_bytes(instruction.source);

        // NOTE:    it will never happen, that source and destination need additional 
        //          bytes
        if(instruction.destination.type == GB_OPERAND_REGISTER_OFFSET)
        {
            instruction.destination.offset8 = state->ram.bytes[state->reg.PC+1];
        }
        else
        {
            if(destination_bytes >= 1)
            {
                instruction.destination.value8 = state->ram.bytes[state->reg.PC+1];
            }
            if(destination_bytes == 2)
            {
                instruction.destination.value8_high = state->ram.bytes[state->reg.PC+2];
            }
        }

        if(instruction.source.type == GB_OPERAND_REGISTER_OFFSET)
        {
            instruction.source.offset8 = state->ram.bytes[state->reg.PC+1];
        }
        else
        {
            if(source_bytes >= 1)
            {
                instruction.source.value8 = state->ram.bytes[state->reg.PC+1];
            }
            if(source_bytes == 2)
            {
                instruction.source.value8_high = state->ram.bytes[state->reg.PC+2];
            }
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

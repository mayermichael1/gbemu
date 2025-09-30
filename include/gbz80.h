#ifndef GBZ80_H
#define GBZ80_H

#include "include/general.h"

/// GB MEMORY

typedef union
{
    u8 registers[12];
    u16 registers_wide[6];
    struct
    {
        union
        {
            u16 AF;
            struct
            {
                u8 F;
                u8 A;
            };
        };
        union
        {
            u16 BC;
            struct
            {
                u8 C;
                u8 B;
            };
        };
        union
        {
            u16 DE;
            struct
            {
                u8 E;
                u8 D;
            };
        };
        union
        {
            u16 HL;
            struct
            {
                u8 L;
                u8 H;
            };
        };
        u16 SP;
        u16 PC;
    };
}
gb_register;

struct gb_vram_tile
{
    u8 data[16];
};

union gb_vram
{
    u8 bytes[(0x9FFF - 0x8000)];
    struct
    {
        union
        {
            struct gb_vram_tile tiles[384];
            struct 
            {
                struct gb_vram_tile tiles0[128];
                struct gb_vram_tile tiles1[128];
                struct gb_vram_tile tiles2[128];
            };
        };
        u8 map0[32 * 32];
        u8 map1[32 * 32];
    };
};

typedef union
{
    u8 bytes[(0xFFFF - 0x0)]; // 64 KB
    struct 
    {
        u8 rom00[(0x3FFF - 0x0)]; // 16 KB
        u8 romN[(0x4000 - 0x3FFF)]; // 16 KB
        union gb_vram vram; // 8 KB
        u8 extram[(0xBFFF - 0xA000)]; // 8 KB
        u8 workram[(0xCFFF - 0xC000)]; // 4 KB
        u8 workram2[(0xEFFF - 0xD000)]; // 4 KB
        u8 echoram[(0xFDFF - 0xE000)]; // 7679 bytes
        u8 oam[(0xFE9F - 0xFE00)]; // 159 bytes
        u8 unusable[(0xFEFF - 0xFEA0)]; // 95 bytes
        u8 io[(0xFF7F - 0xFF00)]; // 172 bytes
        u8 highram[(0xFFFE - 0xFF80)]; // 126 bytes
        u8 interupt[(0xFFFF - 0xFFFE)]; // 1 byte
    };
}
gb_memory;

/// GBASM

typedef enum
{
    GB_OPERAND_NONE,
    GB_OPERAND_ADDRESS,
    GB_OPERAND_REGISTER,
    GB_OPERAND_REGISTER_OFFSET,
    GB_OPERAND_REGISTER_ADDRESS,
    GB_OPERAND_IMMEDIATE,
}
gb_operand_type;

typedef struct
{
    gb_operand_type type;
    b8 operand_wide;

    b8 value_wide;
    union
    {
        u16 value16;
        struct
        {
            u8 value8;
            u8 value8_high;
        };
    };
    u8 offset8;
}
gb_operand;

typedef enum
{
    GB_OPERATION_NONE = 0,
    GB_OPERATION_NOP,
    GB_OPERATION_ADD,
    GB_OPERATION_LOAD,
    GB_OPERATION_LOAD_INCREMENT,
    GB_OPERATION_LOAD_DECREMENT,
    GB_OPERATION_LOAD_HIGH,
    GB_OPERATION_INC,
}
gb_operation_type;

typedef enum
{
    GB_FLAG_ACTION_LEAVE = 0,
    GB_FLAG_ACTION_UNSET,
    GB_FLAG_ACTION_SET,
    GB_FLAG_ACTION_ACCORDINGLY
}
gb_flag_action;

typedef enum
{
    GB_FLAG_ZERO = 7,
    GB_FLAG_SUBTRACTION = 6,
    GB_FLAG_HALF_CARRY = 5,
    GB_FLAG_CARRY = 4,
    GB_FLAG_NONE3 = 3,
    GB_FLAG_NONE2 = 2,
    GB_FLAG_NONE1 = 1,
    GB_FLAG_NONE0 = 0,
}
gb_cpu_flag_bit;

typedef struct
{
    gb_operation_type operation;
    gb_operand destination;
    gb_operand source;
    u8 cycles;
    u8 cycles_worst_case;

    gb_flag_action flag_actions[8];
}
gb_instruction;

global_variable gb_instruction instructions[255];

/// GB STATE

typedef struct
{
    gb_register reg;
    gb_memory ram;
    u64 last_operation_cycle;
    u64 cycle;
    gb_instruction current_instruction;
}
gb_state;


#endif

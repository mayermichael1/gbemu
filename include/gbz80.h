#ifndef GBZ80_H
#define GBZ80_H

#include "include/general.h"

/// GB MEMORY

struct gbz80_register
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
    u16 SP;
    u16 PC;
};

struct gbz80_vram_tile
{
    u8 data[16];
};

union gbz80_vram
{
    u8 bytes[(0x9FFF - 0x8000)];
    struct
    {
        union
        {
            struct gbz80_vram_tile tiles[384];
            struct 
            {
                struct gbz80_vram_tile tiles0[128];
                struct gbz80_vram_tile tiles1[128];
                struct gbz80_vram_tile tiles2[128];
            };
        };
        u8 map0[32 * 32];
        u8 map1[32 * 32];
    };
};

union gbz80_memory
{
    u8 bytes[(0xFFFF - 0x0)]; // 64 KB
    struct 
    {
        u8 rom00[(0x3FFF - 0x0)]; // 16 KB
        u8 romN[(0x4000 - 0x3FFF)]; // 16 KB
        union gbz80_vram vram; // 8 KB
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
};

typedef struct
{
    struct gbz80_register reg;
    union gbz80_memory ram;
}
gbz80_state;


/// GBASM

typedef enum
{
    GB_OPERAND_NONE,
    GB_OPERAND_VALUE,
    GB_OPERAND_REGISTER
}
gb_operand_type;

typedef struct
{
    gb_operand_type type;
    union
    {
        u16 value16;
        u8 value8;
    };
}
gb_operand;

typedef enum
{
    GB_OPERATION_NONE = 0,
    GB_OPERATION_NOP,
    GB_OPERATION_ADD,
}
gb_operation_type;

typedef struct
{
    gb_operation_type operation;
    gb_operand operand_a;
    gb_operand operand_b;
    u8 cycles;
    u8 cycles_worst_case;
    u8 instruction_size;
}
gb_instruction;

global_variable gb_instruction instructions[255];


#endif

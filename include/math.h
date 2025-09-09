#ifndef MATH_H
#define MATH_H

#include "general.h"

typedef union
{
    struct 
    {
        u32 x;
        u32 y;
    };
    struct 
    {
        u32 width;
        u32 height;
    };
}v2u32;

typedef struct
{
    u8 r;
    u8 g;
    u8 b;
    u8 a;
}v4u8;

typedef struct
{
    f32 r;
    f32 g;
    f32 b;
    f32 a;
}v4f32;

#endif

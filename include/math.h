#ifndef MATH_H
#define MATH_H

#include "general.h"

union v2u32
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
};

struct v4u8
{
    u8 r;
    u8 g;
    u8 b;
    u8 a;
};

struct v4f32
{
    f32 r;
    f32 g;
    f32 b;
    f32 a;
};

#endif

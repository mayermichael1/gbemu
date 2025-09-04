#ifndef MEMORY_H
#define MEMORY_H

#include "platform.h"

#define KB  1024
#define MB  KB * KB
#define GB  MB * MB

struct temp_memory
{
    umm start;
    umm current;
    umm end;
};

temp_memory create_temp_memory(umm size)
{
    temp_memory scratch = {};
    scratch.start = allocate(size);
    scratch.end = scratch.start + size;
    scratch.current = scratch.start;
    return scratch;
}

umm
temp_memory_push(temp_memory *scratch, umm size)
{
    ASSERT((scratch->end - scratch->current) >= size);
    umm address = scratch->current;
    scratch->current += size;
    return address;
}

void
destroy_temp_memory(temp_memory *scratch)
{
    deallocate(scratch->start, (scratch->end - scratch->start));
    scratch->start = 0;
    scratch->end = 0;
    scratch->current = 0;
}

#endif 

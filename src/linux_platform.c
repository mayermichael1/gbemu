#include "include/platform.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

u64 
get_file_size(const char *filename)
{
    struct stat st;
    stat(filename, &st);

    u64 filesize = st.st_size;
    return filesize;
}
void
read_file(const char *filename, u64 file_size, u8 *buffer)
{
    s32 file = open(filename, O_RDONLY);

    if(file > 0)
    {
        read(file, buffer, file_size);
        close(file);
    }
    buffer[file_size] = 0;
}

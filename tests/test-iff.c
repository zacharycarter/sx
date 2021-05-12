#include "sx/allocator.h"
#include "sx/io.h"
#include "sx/string.h"

#include <stdio.h>

static const uint8_t dummy_data[473] =
{
    0x7b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x22, 0x66, 0x6f, 0x6c, 0x64, 0x65, 0x72, 0x73, 0x22, 0x3a, 
    0x20, 0x5b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7b, 0x0a, 0x20, 0x20, 0x20, 
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x70, 0x61, 0x74, 0x68, 0x22, 0x3a, 
    0x20, 0x22, 0x2e, 0x22, 0x2c, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
    0x20, 0x20, 0x22, 0x66, 0x6f, 0x6c, 0x64, 0x65, 0x72, 0x5f, 0x65, 0x78, 0x63, 0x6c, 0x75, 0x64, 
    0x65, 0x5f, 0x70, 0x61, 0x74, 0x74, 0x65, 0x72, 0x6e, 0x73, 0x22, 0x3a, 0x20, 0x5b, 0x0a, 0x20, 
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 
    0x62, 0x75, 0x69, 0x6c, 0x64, 0x22, 0x2c, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x2e, 0x76, 0x73, 0x63, 0x6f, 0x64, 0x65, 
    0x22, 0x2c, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
    0x20, 0x20, 0x20, 0x22, 0x2e, 0x63, 0x6c, 0x61, 0x6e, 0x67, 0x64, 0x22, 0x2c, 0x0a, 0x20, 0x20, 
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x62, 
    0x69, 0x6e, 0x22, 0x2c, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
    0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x62, 0x75, 0x69, 0x6c, 0x64, 0x2d, 0x2a, 0x22, 0x0a, 0x20, 
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5d, 0x2c, 0x0a, 0x20, 0x20, 
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x66, 0x69, 0x6c, 0x65, 0x5f, 
    0x65, 0x78, 0x63, 0x6c, 0x75, 0x64, 0x65, 0x5f, 0x70, 0x61, 0x74, 0x74, 0x65, 0x72, 0x6e, 0x73, 
    0x22, 0x3a, 0x20, 0x5b, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
    0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x2e, 0x67, 0x69, 0x74, 0x69, 0x67, 0x6e, 0x6f, 0x72, 0x65, 
    0x22, 0x2c, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
    0x20, 0x20, 0x20, 0x22, 0x2e, 0x74, 0x72, 0x61, 0x76, 0x69, 0x73, 0x2e, 0x79, 0x6d, 0x6c, 0x22, 
    0x2c, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
    0x20, 0x20, 0x22, 0x2e, 0x67, 0x69, 0x74, 0x6d, 0x6f, 0x64, 0x75, 0x6c, 0x65, 0x73, 0x22, 0x2c, 
    0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
    0x20, 0x22, 0x2e, 0x63, 0x6c, 0x61, 0x6e, 0x67, 0x2d, 0x66, 0x6f, 0x72, 0x6d, 0x61, 0x74, 0x22, 
    0x2c, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
    0x20, 0x20, 0x22, 0x63, 0x6f, 0x6d, 0x70, 0x69, 0x6c, 0x65, 0x5f, 0x63, 0x6f, 0x6d, 0x6d, 0x61, 
    0x6e, 0x64, 0x73, 0x2e, 0x6a, 0x73, 0x6f, 0x6e, 0x22, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5d, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
    0x7d, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x5d, 0x0a, 0x7d,                                           
};

#define LEVEL1_FOURCC sx_makefourcc('L', 'V', 'L', '1')
#define LEVEL2_FOURCC sx_makefourcc('L', 'V', 'L', '2')

static void test_write_iff(const char* filename) 
{
    sx_file f;
    if (sx_file_open(&f, filename, SX_FILE_WRITE)) {
        sx_iff_file iff;
        sx_iff_init_from_file_writer(&iff, &f, 0, sx_alloc_malloc());
        
        int level1 = sx_iff_put_chunk(&iff, 0, LEVEL1_FOURCC, dummy_data, sizeof(dummy_data), 0, 0);
        sx_iff_put_chunk(&iff, level1, LEVEL2_FOURCC, dummy_data, sizeof(dummy_data), 0, 0);
        sx_iff_put_chunk(&iff, level1, LEVEL2_FOURCC, dummy_data, sizeof(dummy_data), 0, 0);
        sx_iff_put_chunk(&iff, 0, LEVEL1_FOURCC, dummy_data, sizeof(dummy_data), 0, 0);
        
        sx_iff_release(&iff);
        sx_file_close(&f);
    } 
}

static void test_append_iff(const char* filename) 
{
    sx_file f;
    if (sx_file_open(&f, filename, SX_FILE_WRITE|SX_FILE_APPEND)) {
        sx_iff_file iff;
        sx_iff_init_from_file_writer(&iff, &f, SX_IFFFLAG_APPEND|SX_IFFFLAG_READ_ALL_CHUNKS, sx_alloc_malloc());
        
        int level1 = sx_iff_put_chunk(&iff, 0, LEVEL1_FOURCC, dummy_data, sizeof(dummy_data), 0, 0);
        sx_iff_put_chunk(&iff, level1, LEVEL2_FOURCC, dummy_data, sizeof(dummy_data), 0, 0);
        sx_iff_put_chunk(&iff, level1, LEVEL2_FOURCC, dummy_data, sizeof(dummy_data), 0, 0);
        sx_iff_put_chunk(&iff, 0, LEVEL1_FOURCC, dummy_data, sizeof(dummy_data), 0, 0);
        
        sx_iff_release(&iff);
        sx_file_close(&f);
    } 
}

static const char* get_tabs(int depth) 
{
    static char depth_str[256];
    for (int i = 0; i < depth; i++) {
        depth_str[i] = '\t';
    }
    depth_str[depth] = '\0';
    return depth_str;
}

static void test_load_iff(const char* filename) 
{
    sx_file f;
    if (sx_file_open(&f, filename, SX_FILE_READ)) {
        sx_iff_file iff;
        sx_iff_init_from_file_reader(&iff, &f, 0, sx_alloc_malloc());

        int depth = 0;
        int level1 = sx_iff_get_chunk(&iff, LEVEL1_FOURCC, 0);
        while (level1 != -1) {
            printf("%sChunk [Level%d] - size: %u (parent: %d)\n", 
                get_tabs(depth), depth+1, (uint32_t)iff.chunks[level1].size, iff.chunks[level1].parent_id);

            depth++;
            int level2 = sx_iff_get_chunk(&iff, LEVEL2_FOURCC, level1);
            while (level2 != -1) {
                printf("%sChunk [level%d] - size: %u (parent: %d)\n", 
                    get_tabs(depth), depth+1, (uint32_t)iff.chunks[level2].size, iff.chunks[level2].parent_id);
                level2 = sx_iff_get_next_chunk(&iff, level2);
            }
            depth--;

            level1 = sx_iff_get_next_chunk(&iff, level1);
        }
    }
}

int main(int argc, char* argv[]) 
{
    if (argc != 2) {
        puts("You must provide a command and a file:");
        puts("  test-iff load");
        puts("  test-iff save");
        return -1;
    }

    const char* command = argv[1];

    if (sx_strequal(command, "save")) {
        test_write_iff("test.bin");
    } else if (sx_strequal(command, "load")) {
        test_load_iff("test.bin");
    } else if (sx_strequal(command, "append")) {
        test_append_iff("test.bin");
    }

    return 0;
}
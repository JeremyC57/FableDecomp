// behaviour test for 00ca1750: the destroy helper zeroes the whole 0x460-byte block before handing it to
// free (checked by a free stand-in that inspects the bytes); a null block is ignored.
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct SBlock0x460 {
    unsigned char m_bytes[0x460];
};

static long g_freed = 0;
static long g_dirty = 0;

static void CheckedFree(void* block)
{
    long i = 0;
    const unsigned char* bytes = (const unsigned char*)block;
    for (i = 0; i < 0x460; ++i) {
        if (bytes[i] != 0)
            ++g_dirty;
    }
    ++g_freed;
    std::free(block);
}

static void ClearAndFreeBlock(SBlock0x460* block)
{
    if (block) {
        std::memset(block, 0, sizeof(SBlock0x460));
        CheckedFree(block);
    }
}

int main()
{
    int ok = 1;
    SBlock0x460* block = (SBlock0x460*)std::malloc(sizeof(SBlock0x460));
    std::memset(block, 0xAB, sizeof(SBlock0x460));
    ClearAndFreeBlock(block);
    if (g_freed != 1 || g_dirty != 0)
        ok = 0;
    ClearAndFreeBlock(0);
    if (g_freed != 1)
        ok = 0;
    if (sizeof(SBlock0x460) != 0x460)
        ok = 0;
    if (!ok) {
        std::printf("00ca1750_TEST FAIL\n");
        return 1;
    }
    std::printf("00ca1750_TEST PASS\n");
    return 0;
}
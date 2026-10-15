#include <cstdio>
static unsigned int g_align = 8, g_shift = 3;
#define ALIGNED_SIZE(size) ((((size) + g_align - 1) >> g_shift) << g_shift)
struct Chunk;
struct Area { Area* next; Area* right; Area* prev; Area* parent; Area* left; Chunk* chunk; long size; unsigned char colour; };
struct Alloc;
struct Chunk { long total, nFree, nAlloc, freeSize, largest, smallest; Alloc* owner; bool permanent, demand; Chunk* nextC; Chunk* prevC; Area* firstFree; Area* lastFree; Area* firstAlloc; };
static int g_calls = 0; static Area* g_lastArea = 0;
struct Alloc { unsigned int alignment, shift; Chunk* first; long minChunk; unsigned int flags, internal; Area* root; int made; Chunk c;
    Chunk* NewChunk(long) { made = 1; return &c; } Chunk* NewChunkDemandPaged(void*, long) { made = 2; return &c; }
    Alloc(long minChunkSize, void* mem, long size, unsigned int alignment_, unsigned int f, unsigned int i)
        : alignment(alignment_), shift(alignment_ == 16 ? 4 : 3), first(0), minChunk(minChunkSize), flags(f), internal(i), root(0), made(0)
    {
        c.permanent = false; g_shift = shift; g_align = alignment;
        if (size != 0) { if (mem != 0) NewChunkDemandPaged(mem, size)->permanent = true; else NewChunk(size)->permanent = true; }
    } };
int main()
{
    Alloc a(100, 0, 0, 16, 1, 2);
    if (a.made != 0 || g_align != 16 || g_shift != 4 || a.minChunk != 100 || a.flags != 1 || a.internal != 2 || a.first != 0) { std::printf("00a65930_TEST FAIL\n"); return 1; }
    Alloc b(100, 0, 64, 8, 0, 0); static long buf[16]; Alloc c(100, buf, 64, 8, 0, 0);
    if (b.made != 1 || !b.c.permanent || c.made != 2 || !c.c.permanent) { std::printf("00a65930_TEST FAIL\n"); return 1; }
    std::printf("00a65930_TEST PASS\n");
    return 0;
}

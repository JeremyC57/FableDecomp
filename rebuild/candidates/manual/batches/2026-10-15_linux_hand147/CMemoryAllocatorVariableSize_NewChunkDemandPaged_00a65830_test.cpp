#include <cstdio>
static unsigned int g_align = 8, g_shift = 3;
#define ALIGNED_SIZE(size) ((((size) + g_align - 1) >> g_shift) << g_shift)
struct Chunk;
struct Area { Area* next; Area* right; Area* prev; Area* parent; Area* left; Chunk* chunk; long size; unsigned char colour; };
struct Alloc;
struct Chunk { long total, nFree, nAlloc, freeSize, largest, smallest; Alloc* owner; bool permanent, demand; Chunk* nextC; Chunk* prevC; Area* firstFree; Area* lastFree; Area* firstAlloc; };
static int g_calls = 0; static Area* g_lastArea = 0;
struct Alloc { Chunk* first; };
static void* g_mem; static long g_size;
static void Init(Chunk*, void* mem, long size, Alloc*) { g_mem = mem; g_size = size; }
static Chunk* NewChunkDemandPaged(Alloc* al, void* memory, long size)
{
    Chunk* c = (Chunk*)memory; long areaSize = size - (long)ALIGNED_SIZE(sizeof(Chunk)); c->demand = true;
    Init(c, (char*)c + ALIGNED_SIZE(sizeof(Chunk)), areaSize, al);
    c->nextC = al->first; if (al->first) al->first->prevC = c; c->prevC = 0; al->first = c; return c;
}
int main()
{
    static long b1[64], b2[64]; Alloc al; al.first = 0;
    Chunk* c1 = NewChunkDemandPaged(&al, b1, 256);
    if (c1 != (Chunk*)b1 || !c1->demand || al.first != c1 || c1->nextC != 0 || g_mem != (char*)b1 + ALIGNED_SIZE(sizeof(Chunk)) || g_size != 256 - (long)ALIGNED_SIZE(sizeof(Chunk))) { std::printf("00a65830_TEST FAIL\n"); return 1; }
    Chunk* c2 = NewChunkDemandPaged(&al, b2, 256);
    if (al.first != c2 || c2->nextC != c1 || c1->prevC != c2 || c2->prevC != 0) { std::printf("00a65830_TEST FAIL\n"); return 1; }
    std::printf("00a65830_TEST PASS\n");
    return 0;
}

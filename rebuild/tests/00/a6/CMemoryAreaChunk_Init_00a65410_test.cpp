#include <cstdio>
static unsigned int g_align = 8, g_shift = 3;
#define ALIGNED_SIZE(size) ((((size) + g_align - 1) >> g_shift) << g_shift)
struct Chunk;
struct Area { Area* next; Area* right; Area* prev; Area* parent; Area* left; Chunk* chunk; long size; unsigned char colour; };
struct Alloc;
struct Chunk { long total, nFree, nAlloc, freeSize, largest, smallest; Alloc* owner; bool permanent, demand; Chunk* nextC; Chunk* prevC; Area* firstFree; Area* lastFree; Area* firstAlloc; };
static int g_calls = 0; static Area* g_lastArea = 0;
static void InsertFree(Chunk*, Area* a) { ++g_calls; g_lastArea = a; }
static void Init(Chunk* c, void* mem, long size, Alloc* parent)
{
    c->owner = parent; c->nextC = 0; c->prevC = 0; c->firstFree = 0; c->lastFree = 0; c->firstAlloc = 0;
    c->nFree = 0; c->nAlloc = 0; c->largest = 0; c->smallest = 0;
    long dataSize = size - (long)ALIGNED_SIZE(sizeof(Area));
    Area* a = (Area*)mem; a->next = 0; a->right = 0; a->prev = 0; a->parent = 0; a->left = 0; a->colour = 0; a->chunk = c; a->size = dataSize;
    c->total = dataSize; c->freeSize = 0; InsertFree(c, a);
}
int main()
{
    static long buf[64]; Chunk c; Init(&c, buf, 200, 0);
    Area* a = (Area*)buf;
    if (g_calls != 1 || g_lastArea != a || a->chunk != &c || a->size != 200 - (long)ALIGNED_SIZE(sizeof(Area)) || c.total != a->size || c.freeSize != 0 || c.firstFree != 0) { std::printf("00a65410_TEST FAIL\n"); return 1; }
    std::printf("00a65410_TEST PASS\n");
    return 0;
}
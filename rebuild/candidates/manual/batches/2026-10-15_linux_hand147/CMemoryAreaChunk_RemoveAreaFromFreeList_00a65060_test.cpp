#include <cstdio>
static unsigned int g_align = 8, g_shift = 3;
#define ALIGNED_SIZE(size) ((((size) + g_align - 1) >> g_shift) << g_shift)
struct Chunk;
struct Area { Area* next; Area* right; Area* prev; Area* parent; Area* left; Chunk* chunk; long size; unsigned char colour; };
struct Alloc;
struct Chunk { long total, nFree, nAlloc, freeSize, largest, smallest; Alloc* owner; bool permanent, demand; Chunk* nextC; Chunk* prevC; Area* firstFree; Area* lastFree; Area* firstAlloc; };
static int g_calls = 0; static Area* g_lastArea = 0;
static void RemoveFromTree(Area* a) { ++g_calls; g_lastArea = a; }
static void RemoveAreaFromFreeList(Chunk* c, Area* a)
{
    if (a->prev) a->prev->next = a->next; else c->firstFree = a->next;
    if (a->next) a->next->prev = a->prev; else c->lastFree = a->prev;
    a->next = 0; a->prev = 0; RemoveFromTree(a);
    --c->nFree; c->freeSize -= a->size;
    if (c->lastFree) c->largest = c->lastFree->size; else c->largest = 0;
    if (c->firstFree) c->smallest = c->firstFree->size; else c->smallest = 0;
}
int main()
{
    Area x = Area(), y = Area(), z = Area(); x.size = 10; y.size = 20; z.size = 30;
    x.next = &y; y.prev = &x; y.next = &z; z.prev = &y;
    Chunk c = Chunk(); c.firstFree = &x; c.lastFree = &z; c.nFree = 3; c.freeSize = 60;
    RemoveAreaFromFreeList(&c, &z);
    if (c.lastFree != &y || y.next != 0 || c.nFree != 2 || c.freeSize != 30 || c.largest != 20 || c.smallest != 10 || g_lastArea != &z) { std::printf("00a65060_TEST FAIL\n"); return 1; }
    RemoveAreaFromFreeList(&c, &x); RemoveAreaFromFreeList(&c, &y);
    if (c.firstFree != 0 || c.lastFree != 0 || c.largest != 0 || c.smallest != 0 || c.freeSize != 0 || g_calls != 3) { std::printf("00a65060_TEST FAIL\n"); return 1; }
    std::printf("00a65060_TEST PASS\n");
    return 0;
}

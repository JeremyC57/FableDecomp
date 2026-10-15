#include <cstdio>
static unsigned int g_align = 8, g_shift = 3;
#define ALIGNED_SIZE(size) ((((size) + g_align - 1) >> g_shift) << g_shift)
struct Chunk;
struct Area { Area* next; Area* right; Area* prev; Area* parent; Area* left; Chunk* chunk; long size; unsigned char colour; };
struct Alloc;
struct Chunk { long total, nFree, nAlloc, freeSize, largest, smallest; Alloc* owner; bool permanent, demand; Chunk* nextC; Chunk* prevC; Area* firstFree; Area* lastFree; Area* firstAlloc; };
static int g_calls = 0; static Area* g_lastArea = 0;
struct Alloc { Chunk* first;
    unsigned int GetAll(void** out, unsigned int) { unsigned int n = 0;
        for (Chunk* c = first; c; c = c->nextC) for (Area* a = c->firstAlloc; a; a = a->next) out[n++] = (char*)a + ALIGNED_SIZE(sizeof(Area));
        return n; } };
int main()
{
    Area a1 = Area(), a2 = Area(), a3 = Area(); a1.next = &a2; Chunk c1 = Chunk(), c2 = Chunk(), c3 = Chunk();
    c1.firstAlloc = &a1; c1.nextC = &c2; c2.nextC = &c3; c3.firstAlloc = &a3;
    Alloc al; al.first = &c1; void* out[8]; unsigned int n = al.GetAll(out, 8);
    if (n != 3 || out[0] != (char*)&a1 + ALIGNED_SIZE(sizeof(Area)) || out[1] != (char*)&a2 + ALIGNED_SIZE(sizeof(Area)) || out[2] != (char*)&a3 + ALIGNED_SIZE(sizeof(Area))) { std::printf("00a65260_TEST FAIL\n"); return 1; }
    al.first = 0; if (al.GetAll(out, 8) != 0) { std::printf("00a65260_TEST FAIL\n"); return 1; }
    std::printf("00a65260_TEST PASS\n");
    return 0;
}
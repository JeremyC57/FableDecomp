// Behaviour test for 0x00A74EA0 CArray<C2DLineF>::DeallocateUnusedMemory: with spare capacity the lines move into an
// exactly sized block (capacity == size afterwards, contents and order kept, the old block freed); a full array
// (capacity == size) is left untouched.
#include <cstdio>
#include <cstdlib>
struct SLine { float x0, y0, x1, y1; };
struct SArray { SLine* start; SLine* finish; SLine* end; };
static int g_blocks = 0;
static SLine* Alloc(unsigned int n) { if (!n) return 0; ++g_blocks; return (SLine*)std::malloc(n * sizeof(SLine)); }
static void Free(SLine* p) { if (p) { --g_blocks; std::free(p); } }
// the function under test (same shape as the source: reserve + insert into a temporary, release, swap, destroy temp)
static void DeallocateUnusedMemory(SArray* a)
{
    if ((unsigned int)(a->end - a->start) > (unsigned int)(a->finish - a->start)) {
        SArray temp = { 0, 0, 0 };
        unsigned int n = (unsigned int)(a->finish - a->start);
        temp.start = Alloc(n); temp.finish = temp.start; temp.end = temp.start + n;
        for (SLine* s = a->start; s != a->finish; ++s) *temp.finish++ = *s;
        SLine* old = a->start;                         // DeallocateMemory: erase + detach + free
        a->start = a->finish = a->end = 0;
        Free(old);
        SLine* t;                                       // swap
        t = a->start; a->start = temp.start; temp.start = t;
        t = a->finish; a->finish = temp.finish; temp.finish = t;
        t = a->end; a->end = temp.end; temp.end = t;
        Free(temp.start);                               // ~temp
    }
}
int main()
{
    SArray a; a.start = Alloc(10); a.finish = a.start + 3; a.end = a.start + 10;
    for (int i = 0; i < 3; ++i) { a.start[i].x0 = (float)i; a.start[i].y1 = (float)(i * 10); }
    DeallocateUnusedMemory(&a);
    if (a.end - a.start != 3 || a.finish - a.start != 3 || g_blocks != 1) { std::printf("00a74ea0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i)
        if (a.start[i].x0 != (float)i || a.start[i].y1 != (float)(i * 10)) { std::printf("00a74ea0_TEST FAIL\n"); return 1; }
    SLine* keep = a.start;
    DeallocateUnusedMemory(&a);                         // already tight: nothing happens
    if (a.start != keep || g_blocks != 1) { std::printf("00a74ea0_TEST FAIL\n"); return 1; }
    SArray e; e.start = Alloc(4); e.finish = e.start; e.end = e.start + 4;
    DeallocateUnusedMemory(&e);                         // empty with capacity: storage released entirely
    if (e.start != 0 || e.finish != 0 || e.end != 0 || g_blocks != 1) { std::printf("00a74ea0_TEST FAIL\n"); return 1; }
    Free(a.start);
    std::printf("00a74ea0_TEST PASS\n");
    return 0;
}
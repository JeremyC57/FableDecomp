// behaviour test for 0x00A4A390 vector<CLipSyncPhoneme >::_M_clear: every element in [start, finish) is destroyed once, front to back, then the block is freed (nothing freed for an empty, never-allocated vector)
#include <cstdio>
#include <cstdlib>
static int gOrder[8]; static int gCount = 0; static int gFreed = 0;
struct Elem { int key; int id; void Destroy() { gOrder[gCount++] = id; } };
struct Vec {
    Elem* start; Elem* finish; Elem* eos;
    void DestroyAndFree()                                    // the function under test
    {
        for (Elem* p = start; p != finish; ++p) p->Destroy();
        if (start != 0) { ++gFreed; std::free(start); }
    }
};
int main()
{
    int fail = 0;
    Vec v; v.start = (Elem*)std::malloc(4 * sizeof(Elem)); v.finish = v.start + 3; v.eos = v.start + 4;
    for (int i = 0; i < 3; ++i) { v.start[i].key = 0; v.start[i].id = i + 1; }
    v.DestroyAndFree();
    if (gCount != 3 || gOrder[0] != 1 || gOrder[1] != 2 || gOrder[2] != 3 || gFreed != 1) fail |= 1;
    Vec e; e.start = e.finish = e.eos = 0;
    e.DestroyAndFree();
    if (gCount != 3 || gFreed != 1) fail |= 2;
    if (fail) { std::printf("00a4a390_TEST FAIL %d\n", fail); return 1; }
    std::printf("00a4a390_TEST PASS\n");
    return 0;
}
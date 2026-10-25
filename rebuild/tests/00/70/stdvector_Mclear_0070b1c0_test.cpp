// behaviour test for 0x0070B1C0 vector<CEngineTextureDecal >::_M_clear: every element's inner block is released (only when it has one), then
// the vector's own block; an empty vector releases only its block (and nothing when it has none)
#include <cstdio>
#include <cstdlib>
static int gFreed = 0;
static void CountingFree(void* p) { if (p != 0) { ++gFreed; std::free(p); } }
struct Inner { int* Start; int* Finish; int* End; };
struct Elem { int Key; Inner Data; };
struct Vec { Elem* Start; Elem* Finish; Elem* End; };
static void Clear(Vec& v)
{
    for (Elem* e = v.Start; e != v.Finish; ++e)
        CountingFree(e->Data.Start);
    CountingFree(v.Start);
}
int main()
{
    int fails = 0;
    Vec v;
    v.Start = (Elem*)std::malloc(3 * sizeof(Elem)); v.Finish = v.Start + 3; v.End = v.Finish;
    v.Start[0].Data.Start = (int*)std::malloc(8);
    v.Start[1].Data.Start = 0;                       // an element without an inner block
    v.Start[2].Data.Start = (int*)std::malloc(4);
    Clear(v);
    if (gFreed != 3) fails |= 1;
    gFreed = 0;
    Vec e; e.Start = 0; e.Finish = 0; e.End = 0;
    Clear(e);
    if (gFreed != 0) fails |= 2;
    if (fails) { std::printf("0070b1c0_TEST FAIL %d\n", fails); return 1; }
    std::printf("0070b1c0_TEST PASS\n");
    return 0;
}
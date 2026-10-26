// behaviour test for 0x00AC3A10 CCountedPointer<CArray<C3DTriangle2>>::DeleteData(void*): a null pointer does nothing; otherwise the object's
// vector storage is freed when allocated, then the object itself
#include <cstdio>
#include <cstdlib>
static int gFreed = 0, gDeleted = 0;
struct Vec { int* Start; int* Finish; int* End; };
struct Obj { Vec V; };
static void DeleteData(void* data)                          // the function under test
{
    Obj* p = (Obj*)data;
    if (p) {
        if (p->V.Start) { std::free(p->V.Start); ++gFreed; }
        std::free(p); ++gDeleted;
    }
}
int main()
{
    int fail = 0;
    DeleteData(0);
    if (gFreed != 0 || gDeleted != 0) fail |= 1;
    Obj* a = (Obj*)std::malloc(sizeof(Obj)); a->V.Start = a->V.Finish = a->V.End = 0;
    DeleteData(a);
    if (gFreed != 0 || gDeleted != 1) fail |= 2;
    Obj* b = (Obj*)std::malloc(sizeof(Obj)); b->V.Start = (int*)std::malloc(4 * sizeof(int)); b->V.Finish = b->V.End = b->V.Start + 4;
    DeleteData(b);
    if (gFreed != 1 || gDeleted != 2) fail |= 4;
    if (fail) { std::printf("00ac3a10_TEST FAIL %d\n", fail); return 1; }
    std::printf("00ac3a10_TEST PASS\n");
    return 0;
}
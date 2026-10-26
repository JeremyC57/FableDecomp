// behaviour test for 0x00A812F0 CArray<CANavQuadTreeNode*>::DeallocateMemory: an array with capacity ends with null start / finish / end of
// storage and its block freed exactly once; an array without capacity is left alone
#include <cstdio>
#include <cstdlib>
#include <cstring>
static int gFrees = 0;
struct Array { int* Start; int* Finish; int* EndOfStorage; };
static void DeallocateMemory(Array& a)
{
    if ((unsigned int)(a.EndOfStorage - a.Start) > 0u) {
        int* first = a.Start; int* last = a.Finish;                      // clear(): copy(end, end, begin)
        if (last != a.Finish) std::memmove(first, last, (char*)a.Finish - (char*)last);
        a.Finish = first;
        int* old = a.Start; a.Start = 0; a.Finish = 0; a.EndOfStorage = 0;   // swap with an empty array
        if (old) { std::free(old); ++gFrees; }                          // the temporary's destructor
    }
}
int main()
{
    int fail = 0;
    Array a; a.Start = (int*)std::malloc(4 * sizeof(int)); a.Finish = a.Start + 3; a.EndOfStorage = a.Start + 4;
    DeallocateMemory(a);
    if (a.Start != 0 || a.Finish != 0 || a.EndOfStorage != 0 || gFrees != 1) fail |= 1;
    DeallocateMemory(a);
    if (gFrees != 1) fail |= 2;
    if (fail) { std::printf("00a812f0_TEST FAIL %d\n", fail); return 1; }
    std::printf("00a812f0_TEST PASS\n");
    return 0;
}
// behaviour test for 0x00827470 CCachedVolumes::`scalar deleting destructor': the element range [start, finish) is destroyed once, the storage freed only when allocated, the object freed only when bit 0 of the flags is set, and this is returned
#include <cstdio>
#include <cstdlib>
static int gRanges = 0, gElems = 0, gStorageFreed = 0, gObjectFreed = 0;
struct Elem { int Tag; };
static void DestroyAux(Elem* first, Elem* last) { ++gRanges; for (; first != last; ++first) ++gElems; }
struct Holder {                                         // stands for CCachedVolumes: a vector as the first member
    Elem* Start; Elem* Finish; Elem* EndOfStorage;
    Holder* DeletingDtor(unsigned int flags)            // the function under test
    {
        DestroyAux(Start, Finish);
        if (Start != 0) { ++gStorageFreed; std::free(Start); }
        if (flags & 1) { ++gObjectFreed; std::free(this); }
        return this;
    }
};
int main()
{
    int fail = 0;
    Holder* h = (Holder*)std::malloc(sizeof(Holder));
    h->Start = (Elem*)std::malloc(3 * sizeof(Elem)); h->Finish = h->Start + 3; h->EndOfStorage = h->Finish;
    Holder local; local.Start = 0; local.Finish = 0; local.EndOfStorage = 0;
    if (local.DeletingDtor(0) != &local) fail |= 1;
    if (gRanges != 1 || gElems != 0 || gStorageFreed != 0 || gObjectFreed != 0) fail |= 2;
    h->DeletingDtor(1);
    if (gRanges != 2 || gElems != 3 || gStorageFreed != 1 || gObjectFreed != 1) fail |= 4;
    if (fail) { std::printf("00827470_TEST FAIL %d\n", fail); return 1; }
    std::printf("00827470_TEST PASS\n");
    return 0;
}
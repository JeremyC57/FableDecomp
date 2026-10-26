// behaviour test for 0x0073EA40 CAnimFilterOf2Groups::`scalar deleting destructor'(unsigned int): both members are destroyed (second first),
// the object is freed only when bit 0 of the flags is set, and this is returned either way
#include <cstdio>
#include <cstdlib>
static int gOrder[4], gN = 0, gFreed = 0;
struct Obj { int m1; int m2; };
static void* DeletingDtor(Obj* p, unsigned int flags)       // the function under test
{
    gOrder[gN++] = p->m2;
    gOrder[gN++] = p->m1;
    if (flags & 1) { std::free(p); ++gFreed; }
    return p;
}
int main()
{
    int fail = 0;
    Obj local; local.m1 = 1; local.m2 = 2;
    if (DeletingDtor(&local, 0) != &local) fail |= 1;
    if (gN != 2 || gOrder[0] != 2 || gOrder[1] != 1 || gFreed != 0) fail |= 2;
    Obj* heap = (Obj*)std::malloc(sizeof(Obj)); heap->m1 = 3; heap->m2 = 4;
    DeletingDtor(heap, 1);
    if (gN != 4 || gOrder[2] != 4 || gOrder[3] != 3 || gFreed != 1) fail |= 4;
    if (sizeof(Obj) != (unsigned int)8) fail |= 8;
    if (fail) { std::printf("0073ea40_TEST FAIL %d\n", fail); return 1; }
    std::printf("0073ea40_TEST PASS\n");
    return 0;
}
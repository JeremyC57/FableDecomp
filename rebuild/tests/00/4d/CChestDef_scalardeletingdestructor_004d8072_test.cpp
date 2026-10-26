// behaviour test for 0x004D8072 CChestDef::`scalar deleting destructor': destroying through a base pointer runs the member's destructor,
// then the base destructors from most to least derived; the memory is freed only when bit 0 of the flags is set (delete), not for a plain destruction
#include <cstdio>
#include <new>
static int gLog[8]; static int gN = 0; static int gFreed = 0;
struct Member { ~Member() { gLog[gN++] = 1; } };
struct Root { virtual ~Root() { gLog[gN++] = 3; } };
struct Mid : public Root { virtual ~Mid() { gLog[gN++] = 2; } };
struct Leaf : public Mid { Member m; static void operator delete(void* p) { ++gFreed; ::operator delete(p); } };
int main()
{
    int fail = 0;
    Root* r = new Leaf;
    delete r;                                                   // flags bit 0 set
    if (gN != 3 || gLog[0] != 1 || gLog[1] != 2 || gLog[2] != 3 || gFreed != 1) fail |= 1;
    gN = 0;
    static double storage[8];
    Root* s = new (storage) Leaf;
    s->~Root();                                                 // flags 0: destroy only
    if (gN != 3 || gLog[0] != 1 || gLog[1] != 2 || gLog[2] != 3 || gFreed != 1) fail |= 2;
    if (fail) { std::printf("004d8072_TEST FAIL %d\n", fail); return 1; }
    std::printf("004d8072_TEST PASS\n");
    return 0;
}
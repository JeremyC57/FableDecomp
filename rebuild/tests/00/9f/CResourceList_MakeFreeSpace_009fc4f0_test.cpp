// Behaviour test for 0x009FC4F0 CResourceList::MakeFreeSpace (links the source object): ShrinkResources is called with the
// same byte count, then the result is AllocatedMemory + bytes <= MaximumMemory (unsigned).
#include <cstdio>
#include <cstring>
#include "engine/CResourceList.h"

struct CResourceList_Methods : CResourceList {
    void ShrinkResources(unsigned long bytes);
    bool MakeFreeSpace(unsigned long bytes);
};
static int g_calls = 0; static unsigned long g_arg = 0;
void CResourceList_Methods::ShrinkResources(unsigned long bytes) { ++g_calls; g_arg = bytes; }

int main()
{
    CResourceList_Methods l; std::memset(&l, 0, sizeof(l));
    l.AllocatedMemory = 90; l.MaximumMemory = 100;
    bool a = l.MakeFreeSpace(10);
    unsigned long first = g_arg;
    bool b = l.MakeFreeSpace(11);
    if (!a || b || g_calls != 2 || first != 10 || g_arg != 11) { std::printf("009fc4f0_TEST FAIL\n"); return 1; }
    std::printf("009fc4f0_TEST PASS\n");
    return 0;
}

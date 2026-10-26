// behaviour test for 0x00B49AA0 CEngineLightingManager::CLocalLightElementLink::Delete(): the block goes on the front of its heap's free list, the live count drops, and the heap is released exactly when the count reaches 0 with release enabled
#include <cstdio>
struct Entry { Entry* NextEntry; int pad[3]; };
struct Heap {
    int AllocCount; bool RefCountReleaseEnabled; Entry* FreeList; int Releases;
    void RefCountedRelease() { ++Releases; }
    void Free(void* object)                                      // the function under test (CLocalLightElementLink's heap free)
    {
        --AllocCount;
        ((Entry*)object)->NextEntry = FreeList;
        FreeList = (Entry*)object;
        if (AllocCount == 0 && RefCountReleaseEnabled)
            RefCountedRelease();
    }
};
int main()
{
    int fail = 0;
    Entry a, b, old;
    Heap h; h.AllocCount = 2; h.RefCountReleaseEnabled = true; h.FreeList = &old; h.Releases = 0;
    h.Free(&a);
    if (h.AllocCount != 1 || h.FreeList != &a || a.NextEntry != &old || h.Releases != 0) fail |= 1;
    h.Free(&b);
    if (h.AllocCount != 0 || h.FreeList != &b || b.NextEntry != &a || h.Releases != 1) fail |= 2;
    Heap g; g.AllocCount = 1; g.RefCountReleaseEnabled = false; g.FreeList = 0; g.Releases = 0;
    g.Free(&a);
    if (g.AllocCount != 0 || g.FreeList != &a || a.NextEntry != 0 || g.Releases != 0) fail |= 4;
    if (fail) { std::printf("00b49aa0_TEST FAIL %d\n", fail); return 1; }
    std::printf("00b49aa0_TEST PASS\n");
    return 0;
}
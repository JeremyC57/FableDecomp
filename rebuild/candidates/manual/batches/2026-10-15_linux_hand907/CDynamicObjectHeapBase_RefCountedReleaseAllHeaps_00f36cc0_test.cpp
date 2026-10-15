#include <cstdio>
// Behaviour test for CDynamicObjectHeapBase::RefCountedReleaseAllHeaps (0x00F36CC0): every listed heap is armed; empty ones freed.
static int g_freed = 0;
struct CDynamicObjectHeapBase {
    virtual ~CDynamicObjectHeapBase() {}
    virtual void FreeAllBlocks() { ++g_freed; }
    long AllocCount; long TotalAllocatorBlocks; bool RefCountReleaseEnabled; CDynamicObjectHeapBase* HeapNext;
    void RefCountedRelease() { RefCountReleaseEnabled = true; if (AllocCount == 0) FreeAllBlocks(); }
};
CDynamicObjectHeapBase* g_pHeapList_0143e964 = 0;
static void RefCountedReleaseAllHeaps() { for (CDynamicObjectHeapBase* heap = g_pHeapList_0143e964; heap != 0; heap = heap->HeapNext) heap->RefCountedRelease(); }
int main()
{
    CDynamicObjectHeapBase a, b, c; a.AllocCount = 0; b.AllocCount = 2; c.AllocCount = 0;
    a.RefCountReleaseEnabled = b.RefCountReleaseEnabled = c.RefCountReleaseEnabled = false;
    a.HeapNext = &b; b.HeapNext = &c; c.HeapNext = 0; g_pHeapList_0143e964 = &a;
    RefCountedReleaseAllHeaps();
    int ok = a.RefCountReleaseEnabled && b.RefCountReleaseEnabled && c.RefCountReleaseEnabled && g_freed == 2;
    g_pHeapList_0143e964 = 0; RefCountedReleaseAllHeaps(); if (g_freed != 2) ok = 0;
    std::printf(ok ? "00f36cc0_TEST PASS\n" : "00f36cc0_TEST FAIL\n"); return ok ? 0 : 1;
}

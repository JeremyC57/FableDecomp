#include <cstdio>
// Behaviour test for CDynamicObjectHeapBase::RefCountedRelease (0x00F36CA0): arms the flag; frees blocks only when nothing is allocated.
static int g_freed = 0;
struct CDynamicObjectHeapBase {
    virtual ~CDynamicObjectHeapBase() {}
    virtual void FreeAllBlocks() { ++g_freed; }
    long AllocCount; long TotalAllocatorBlocks; bool RefCountReleaseEnabled; CDynamicObjectHeapBase* HeapNext;
    void RefCountedRelease() { RefCountReleaseEnabled = true; if (AllocCount == 0) FreeAllBlocks(); }
};
int main()
{
    CDynamicObjectHeapBase h; h.AllocCount = 3; h.RefCountReleaseEnabled = false;
    h.RefCountedRelease(); int ok = h.RefCountReleaseEnabled && g_freed == 0;
    h.AllocCount = 0; h.RefCountedRelease(); if (g_freed != 1) ok = 0;
    std::printf(ok ? "00f36ca0_TEST PASS\n" : "00f36ca0_TEST FAIL\n"); return ok ? 0 : 1;
}

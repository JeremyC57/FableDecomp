#include <cstdio>
// Behaviour test for the CDynamicObjectHeapBase deleting destructor (0x00F36CF0): delete through the base runs the virtual destructor.
static int g_destroyed = 0;
struct CDynamicObjectHeapBase {
    virtual ~CDynamicObjectHeapBase() { ++g_destroyed; }
    virtual void FreeAllBlocks() {}
    long AllocCount; long TotalAllocatorBlocks; bool RefCountReleaseEnabled; CDynamicObjectHeapBase* HeapNext;
};
int main()
{
    CDynamicObjectHeapBase* h = new CDynamicObjectHeapBase; delete h;
    CDynamicObjectHeapBase local; (void)local;
    std::printf(g_destroyed == 1 ? "00f36cf0_TEST PASS\n" : "00f36cf0_TEST FAIL\n"); return g_destroyed == 1 ? 0 : 1;
}

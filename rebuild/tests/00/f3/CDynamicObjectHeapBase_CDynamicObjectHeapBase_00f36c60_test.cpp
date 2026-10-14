#include <cstdio>
#include <cstring>
#include <cmath>

struct CDynamicObjectHeapBase { virtual void v00() {} long AllocCount; long TotalAllocatorBlocks; bool RefCountReleaseEnabled; CDynamicObjectHeapBase* HeapNext; CDynamicObjectHeapBase(); };
static CDynamicObjectHeapBase* g_list = 0;
CDynamicObjectHeapBase::CDynamicObjectHeapBase() { HeapNext = g_list; g_list = this; RefCountReleaseEnabled = false; AllocCount = 0; TotalAllocatorBlocks = 0; }
int main() {
    CDynamicObjectHeapBase a; CDynamicObjectHeapBase b;
    if (g_list != &b || b.HeapNext != &a || a.HeapNext != 0 || b.AllocCount || b.TotalAllocatorBlocks || b.RefCountReleaseEnabled) { std::printf("00f36c60_TEST FAIL\n"); return 1; }
    std::printf("00f36c60_TEST PASS\n"); return 0;
}
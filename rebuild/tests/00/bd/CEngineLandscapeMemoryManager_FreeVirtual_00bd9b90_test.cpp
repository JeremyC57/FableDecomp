#include <cstdio>
static int g_deallocs = 0, g_dtors = 0, g_locks = 0, g_unlocks = 0; static long g_count = 0;
struct CMemoryAllocator {
    void InternalDeallocate(void*) { ++g_deallocs; --g_count; }
    long GetAllocationCount() { return g_count; }
    ~CMemoryAllocator() { ++g_dtors; }
};
static void GFEnterAllocatorLock() { ++g_locks; }
static void GFLeaveAllocatorLock() { ++g_unlocks; }
static CMemoryAllocator* g_pGlobalAllocator = 0; static bool g_bFreeWhenEmpty = false;
static void FreeVirtual(void* block)
{
    if (block) {
        GFEnterAllocatorLock();
        if (g_pGlobalAllocator) {
            g_pGlobalAllocator->InternalDeallocate(block);
            if (g_bFreeWhenEmpty && g_pGlobalAllocator->GetAllocationCount() == 0) { delete g_pGlobalAllocator; g_pGlobalAllocator = 0; }
        }
        GFLeaveAllocatorLock();
    }
}
int main()
{
    int dummy;
    FreeVirtual(0);
    if (g_locks != 0) { std::printf("00bd9b90_TEST FAIL\n"); return 1; }
    g_pGlobalAllocator = new CMemoryAllocator; g_count = 2;
    FreeVirtual(&dummy);                             // flag off: allocator kept
    if (g_deallocs != 1 || g_pGlobalAllocator == 0 || g_locks != 1 || g_unlocks != 1) { std::printf("00bd9b90_TEST FAIL\n"); return 1; }
    g_bFreeWhenEmpty = true;
    FreeVirtual(&dummy);                             // last allocation: allocator destroyed
    if (g_deallocs != 2 || g_pGlobalAllocator != 0 || g_dtors != 1) { std::printf("00bd9b90_TEST FAIL\n"); return 1; }
    FreeVirtual(&dummy);                             // no allocator: lock/unlock only
    if (g_deallocs != 2 || g_locks != 3 || g_unlocks != 3) { std::printf("00bd9b90_TEST FAIL\n"); return 1; }
    std::printf("00bd9b90_TEST PASS\n");
    return 0;
}
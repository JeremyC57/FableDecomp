#include <cstdio>
static int g_lock = 0, g_unlock = 0, g_dealloc = 0, g_dtor = 0, g_vfree = 0; static long g_count = 0; static void* g_lastBlock = 0;
class CMemoryAllocator { public: void InternalDeallocate(void* b) { ++g_dealloc; g_lastBlock = b; } long GetAllocationCount() { return g_count; } ~CMemoryAllocator() { ++g_dtor; }
    static void operator delete(void* p) { ++g_vfree; ::operator delete(p); } };
static void Enter() { ++g_lock; } static void Leave() { ++g_unlock; }
static CMemoryAllocator* g_alloc = 0; static bool g_destroyWhenEmpty = false;
static void Free(void* block) { Enter(); if (block && g_alloc) { g_alloc->InternalDeallocate(block); if (g_destroyWhenEmpty) { if (g_alloc->GetAllocationCount() == 0) { delete g_alloc; g_alloc = 0; } } } Leave(); }
int main()
{
    int b; Free(&b); if (g_lock != 1 || g_unlock != 1 || g_dealloc != 0) { std::printf("009a4350_TEST FAIL\n"); return 1; }
    g_alloc = new CMemoryAllocator; Free(0); if (g_dealloc != 0) { std::printf("009a4350_TEST FAIL\n"); return 1; }
    g_count = 1; Free(&b); if (g_dealloc != 1 || g_lastBlock != &b || g_dtor != 0) { std::printf("009a4350_TEST FAIL\n"); return 1; }
    g_destroyWhenEmpty = true; Free(&b); if (g_dealloc != 2 || g_dtor != 0) { std::printf("009a4350_TEST FAIL\n"); return 1; }
    g_count = 0; Free(&b); if (g_dealloc != 3 || g_dtor != 1 || g_vfree != 1 || g_alloc != 0 || g_lock != 5 || g_unlock != 5) { std::printf("009a4350_TEST FAIL\n"); return 1; }
    std::printf("009a4350_TEST PASS\n"); return 0;
}
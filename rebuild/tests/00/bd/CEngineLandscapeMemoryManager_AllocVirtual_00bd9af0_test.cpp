#include <cstdio>
static int g_created = 0, g_lock = 0, g_unlock = 0; static long g_size = 0; static unsigned long g_align = 1; static bool g_flag = true; static int g_block;
class CMemoryAllocator { public: void* InternalAllocate(long size, unsigned long align, bool flag) { g_size = size; g_align = align; g_flag = flag; if (g_lock != g_unlock + 1) return 0; return &g_block; } };
static CMemoryAllocator g_a; static CMemoryAllocator* g_alloc = 0;
static void Create() { ++g_created; g_alloc = &g_a; } static void Enter() { ++g_lock; } static void Leave() { ++g_unlock; }
struct CMM { void* AllocVirtual(long size, long) { if (!g_alloc) Create(); Enter(); void* b = g_alloc->InternalAllocate(size, 0, false); Leave(); return b; } };
int main()
{
    CMM m; void* b = m.AllocVirtual(100, 7);
    if (b != &g_block || g_created != 1 || g_size != 100 || g_align != 0 || g_flag || g_lock != 1 || g_unlock != 1) { std::printf("00bd9af0_TEST FAIL\n"); return 1; }
    m.AllocVirtual(5, 0); if (g_created != 1 || g_size != 5 || g_lock != 2 || g_unlock != 2) { std::printf("00bd9af0_TEST FAIL\n"); return 1; }
    std::printf("00bd9af0_TEST PASS\n"); return 0;
}
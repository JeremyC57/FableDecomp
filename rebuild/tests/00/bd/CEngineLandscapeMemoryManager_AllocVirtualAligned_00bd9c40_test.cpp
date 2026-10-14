#include <cstdio>
static int g_created = 0, g_lock = 0, g_unlock = 0; static long g_size = 0; static unsigned char g_buf[256]; static bool g_fail = false;
struct CMemoryAllocator { void* InternalAllocate(long size, unsigned long, bool) { g_size = size; return g_fail ? 0 : g_buf + 1; } };
static CMemoryAllocator g_a; static CMemoryAllocator* g_alloc = 0;
static void Create() { ++g_created; g_alloc = &g_a; } static void Enter() { ++g_lock; } static void Leave() { ++g_unlock; }
struct CMM { void* AllocVirtualAligned(long size, unsigned long alignment, long) {
    if (!g_alloc) Create(); Enter(); unsigned char* block = (unsigned char*)g_alloc->InternalAllocate(size + alignment + 3, 0, false); Leave();
    if (!block) return 0; unsigned char* aligned = (unsigned char*)(((unsigned long)block + alignment + 3) & ~(alignment - 1)); ((unsigned long*)aligned)[-1] = aligned - block; return aligned; } };
int main()
{
    CMM m; unsigned char* a = (unsigned char*)m.AllocVirtualAligned(64, 16, 0);
    if (!a || g_created != 1 || g_size != 64 + 16 + 3 || ((unsigned long)a & 15) != 0 || a <= g_buf + 1 + 3 || a - (g_buf + 1) != (long)((unsigned long*)a)[-1] || g_lock != 1 || g_unlock != 1) { std::printf("00bd9c40_TEST FAIL\n"); return 1; }
    g_fail = true; if (m.AllocVirtualAligned(8, 4, 0) != 0 || g_lock != 2 || g_unlock != 2) { std::printf("00bd9c40_TEST FAIL\n"); return 1; }
    std::printf("00bd9c40_TEST PASS\n"); return 0;
}
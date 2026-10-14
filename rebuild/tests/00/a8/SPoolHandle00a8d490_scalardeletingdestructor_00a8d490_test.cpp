#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_freed = 0; static void* g_pool = 0; struct CPool { void Free(long h) { g_freed = h; g_pool = this; } };
struct SHandle { CPool* m_pPool; long m_handle; ~SHandle() { if (m_pPool) { m_pPool->Free(m_handle); m_pPool = 0; m_handle = -1; } } };

int main()
{
    CPool pool; SHandle* p = (SHandle*)std::malloc(sizeof(SHandle)); p->m_pPool = &pool; p->m_handle = 42; p->~SHandle();
    if (g_freed != 42 || g_pool != &pool || p->m_pPool || p->m_handle != -1) { std::printf("00a8d490_TEST FAIL\n"); return 1; } g_freed = 0; p->~SHandle(); if (g_freed) { std::printf("00a8d490_TEST FAIL\n"); return 1; } std::free(p);
    std::printf("00a8d490_TEST PASS\n");
    return 0;
}
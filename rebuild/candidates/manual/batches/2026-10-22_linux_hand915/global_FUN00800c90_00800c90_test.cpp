#include <cstdio>
#include <cstdlib>
// Behaviour test for 00800c90: the storage is released through free() only when the pointer is non-null.
static int g_freed = 0;
static void CountingFree(void* p) { ++g_freed; std::free(p); }
struct Storage { void* _M_start; void Release() { if (_M_start) CountingFree(_M_start); } };
int main()
{
    Storage s; s._M_start = 0;
    s.Release();
    int ok = (g_freed == 0);
    s._M_start = std::malloc(16);
    s.Release();
    ok = ok && g_freed == 1;
    if (!ok) { std::printf("00800c90_TEST FAIL\n"); return 1; }
    std::printf("00800c90_TEST PASS\n");
    return 0;
}

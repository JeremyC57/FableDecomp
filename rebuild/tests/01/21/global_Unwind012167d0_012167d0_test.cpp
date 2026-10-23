#include <cstdio>
#include <new>
static bool g_use = false; static int g_a = 0, g_alloc = 0; struct Lb { virtual void v0() {} static void* operator new(size_t n) { ++g_alloc; return ::operator new(n); } static void operator delete(void* p) { ::operator delete(p); } }; struct La : Lb { La() { ++g_a; } }; struct Lc : Lb { Lc() {} }; static Lb* g_p = 0; static long g_cnt = 0;
static bool Create() { Lb* l; if (g_use) l = new La; else l = new Lc; g_p = l; if (!l) return false; g_cnt = 1; return true; }
static bool Check() { if (!Create() || g_a != 0 || g_cnt != 1 || !g_p) return false; delete g_p; g_use = true; g_cnt = 0; if (!Create() || g_a != 1 || g_cnt != 1) return false; delete g_p; return g_alloc == 2; }

int main()
{
    if (!Check()) { std::printf("00c74360_TEST FAIL\n"); return 1; }
    std::printf("012167d0_TEST PASS\n");
    return 0;
}
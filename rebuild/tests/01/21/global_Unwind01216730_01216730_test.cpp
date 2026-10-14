#include <cstdio>
#include <new>
typedef long (*PF)(const void*, unsigned long, unsigned long, void*, unsigned long); static long g_prop = 0, g_val = 0, g_size = 0, g_res = 0, g_base = 0; static long Setter(const void* g, unsigned long p, unsigned long s, void* v, unsigned long n) { g_prop = p; g_val = *(long*)v; g_size = n; return 0; } static PF g_pfn = 0; static const unsigned char g_guid[16] = { 0 };
struct Rs { PF Resolve(const char* n) { ++g_res; return Setter; } }; static Rs g_rs; struct Bs { virtual void v0() {} ~Bs() { ++g_base; } }; struct Mk { unsigned char b[4]; ~Mk() {} };
struct X : Bs { bool en; unsigned char p[3]; Mk mk; ~X() { long volume = -10000; if (!g_pfn) g_pfn = g_rs.Resolve("EAXSet"); g_pfn(g_guid, 5, 0, &volume, 4); en = false; } };
static bool Check() { { X x; x.en = true; } { X y; y.en = true; } return g_res == 1 && g_prop == 5 && g_val == -10000 && g_size == 4 && g_base == 2; }

int main()
{
    if (!Check()) { std::printf("00c71cf0_TEST FAIL\n"); return 1; }
    std::printf("00c71cf0_TEST PASS\n");
    return 0;
}
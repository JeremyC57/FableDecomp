// behaviour test for 0x00F29F40: Alloc returns a freshly constructed object, or null when allocation fails
#include <cstdio>
#include <new>
static int g_ctor = 0; static bool g_fail = false;
struct D { unsigned char b[0x1088]; D() { ++g_ctor; b[0] = 1; } };
static unsigned char g_buf[sizeof(D)];
static D* Alloc() { void* p = g_fail ? 0 : g_buf; return p ? new (p) D : 0; }
int main()
{
    D* d = Alloc(); g_fail = true; D* e = Alloc();
    if (!d || d->b[0] != 1 || e || g_ctor != 1) { std::printf("00f29f40_TEST FAIL\n"); return 1; }
    std::printf("00f29f40_TEST PASS\n");
    return 0;
}
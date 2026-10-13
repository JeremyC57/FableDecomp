#include <cstdio>
static long g_k = 0, g_s = 0; struct SP { long a, b; SP() : a(0), b(0) {} };
struct Base { virtual void v00() {} Base(void*, void*, long k, const SP& p) { g_k = k; g_s = p.a + p.b; } };
struct D : Base { virtual void v00() {} D(void* o, void* t) : Base(o, t, 0x8c, SP()) {} };
int main()
{
    D d(0, 0); if (g_k != 0x8c || g_s != 0) { std::printf("00962990_TEST FAIL\n"); return 1; }

    std::printf("00962990_TEST PASS\n");
    return 0;
}

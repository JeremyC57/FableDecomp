#include <cstdio>
static int g_a = 0, g_b = 0; struct M8 { unsigned char b[16]; M8() { ++g_a; } ~M8() {} }; struct MN { unsigned char b[4]; MN(void*) { ++g_b; } ~MN() {} };
struct D { ~D() {} }; struct PB : D { virtual void v() {} void* p; M8 m; PB(void* i) : p(i), m() {} };
struct X : PB { MN n; X(void* s, void* i) : PB(i), n(s) {} };
int main()
{
    int v; X x(&v, &v); if (x.p != &v || g_a != 1 || g_b != 1) { std::printf("00c1fcc0_TEST FAIL\n"); return 1; }

    std::printf("012127e0_TEST PASS\n");
    return 0;
}
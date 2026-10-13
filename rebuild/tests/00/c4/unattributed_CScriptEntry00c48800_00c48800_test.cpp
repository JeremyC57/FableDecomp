#include <cstdio>
static long g_z = 1; struct B { void* d; B() : d(0) {} ~B() {} }; struct M { void* d; M(long z, void* v) : d(v) { g_z = z; } };
struct X : B { unsigned char pad[0x14]; void* f; M m; X(void* a, void* b) : B(), f(a), m(0, b) {} };
int main()
{
    int p, q; X x(&p, &q); if (x.f != &p || x.m.d != &q || g_z != 0) { std::printf("00c48800_TEST FAIL\n"); return 1; }

    std::printf("00c48800_TEST PASS\n");
    return 0;
}
#include <cstdio>
#include <cstdlib>
static int g_f = 0; struct E { void* p; unsigned char r[8]; ~E() { if (p) { std::free(p); ++g_f; } } };
struct V { E* f; E* l; E* e; ~V() { E* ll = l; for (E* p = f; p != ll; ++p) p->~E(); if (f) std::free(f); } };
int main()
{
    { V v; v.f = (E*)std::malloc(sizeof(E) * 2); v.l = v.f + 2; v.e = v.l; v.f[0].p = std::malloc(4); v.f[1].p = 0; } if (g_f != 1) { std::printf("00a74f70_TEST FAIL\n"); return 1; }

    std::printf("00a74f70_TEST PASS\n");
    return 0;
}
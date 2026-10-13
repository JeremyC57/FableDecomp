#include <cstdio>
#include <cstdlib>
static int g_d = 0; struct M { void* d; ~M() { ++g_d; } }; struct E { M a, b; unsigned char r[8]; };
struct V { E* f; E* l; E* e; ~V() { E* ll = l; for (E* p = f; p != ll; ++p) p->~E(); if (f) std::free(f); } };
int main()
{
    { V v; v.f = (E*)std::malloc(sizeof(E) * 2); v.l = v.f + 2; v.e = v.l; } if (g_d != 4) { std::printf("00aeca80_TEST FAIL\n"); return 1; }

    std::printf("00aeca80_TEST PASS\n");
    return 0;
}

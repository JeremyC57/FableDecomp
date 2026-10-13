#include <cstdio>
#include <cstdlib>
#include <new>
static int g_d = 0; struct E { virtual ~E() { ++g_d; } unsigned char b[0x44]; };
struct V { E* f; E* l; E* e; ~V() { E* p = f; E* s = l; for (; p != s; ++p) p->~E(); if (f) std::free(f); } };
int main()
{
    { V v; v.f = (E*)std::malloc(sizeof(E) * 2); new (v.f) E; new (v.f + 1) E; v.l = v.f + 2; v.e = v.l; } if (g_d != 2) { std::printf("00e54c10_TEST FAIL\n"); return 1; }

    std::printf("00e54c10_TEST PASS\n");
    return 0;
}
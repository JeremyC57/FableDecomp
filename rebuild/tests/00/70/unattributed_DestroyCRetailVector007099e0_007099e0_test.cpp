#include <cstdio>
#include <cstdlib>
struct E { unsigned char h[8]; void* d; unsigned char r[8]; };
struct V { E* f; E* l; void D() { E* p = f; E* e = l; for (; p != e; ++p) if (p->d) std::free(p->d); if (f) std::free(f); } };
int main()
{
    V v; v.f = (E*)std::malloc(sizeof(E) * 2); v.l = v.f + 2; v.f[0].d = std::malloc(4); v.f[1].d = 0; v.D(); V w; w.f = 0; w.l = 0; w.D(); if (sizeof(E) != 20) { std::printf("007099e0_TEST FAIL\n"); return 1; }

    std::printf("007099e0_TEST PASS\n");
    return 0;
}
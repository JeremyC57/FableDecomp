#include <cstdio>
#include <new>
static int g_c = 0, g_g = 0; struct E { unsigned char b[4]; E() {} E(const E& o) { ++g_c; b[0] = o.b[0]; } };
struct Tg { Tg() {} }; struct V { E* f; E* l; E* e; void G(E*, const E&, const Tg&, unsigned long, bool) { ++g_g; } void P(const E& v) { if (l != e) { new (l) E(v); ++l; } else G(l, v, Tg(), 1, true); } };
int main()
{
    unsigned char buf[8]; V v; v.f = (E*)buf; v.l = v.f; v.e = v.f + 2; E x; x.b[0] = 9; v.P(x); v.P(x); v.P(x); if (g_c != 2 || g_g != 1 || v.l != v.f + 2 || v.f[1].b[0] != 9) { std::printf("004942c3_TEST FAIL\n"); return 1; }

    std::printf("004942c3_TEST PASS\n");
    return 0;
}

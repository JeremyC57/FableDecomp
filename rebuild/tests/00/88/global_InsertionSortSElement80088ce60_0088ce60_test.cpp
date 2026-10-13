#include <cstdio>
static int g_l = 0; struct E { long a, b; E(long x) : a(x), b(0) {} E(const E& o) : a(o.a), b(o.b) {} }; struct Cmp { void* c; };
static void __fastcall LI(E*, E*, E v, Cmp) { g_l += (int)v.a; }
static void __fastcall IS(E* f, E* l, Cmp c) { if (f != l) for (E* i = f + 1; i != l; ++i) LI(f, i, E(*i), c); }
int main()
{
    E a[3] = { E(1), E(2), E(3) }; Cmp c = { 0 }; IS(a, a + 3, c); if (g_l != 5) { std::printf("0088ce60_TEST FAIL\n"); return 1; }

    std::printf("0088ce60_TEST PASS\n");
    return 0;
}
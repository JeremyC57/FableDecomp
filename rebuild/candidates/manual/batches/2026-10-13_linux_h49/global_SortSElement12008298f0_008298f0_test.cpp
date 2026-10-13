#include <cstdio>
static int g_i = 0, g_f = 0; struct E { unsigned char b[12]; }; struct Cmp { void* c; };
template<class S> inline S Lg(S n) { S k; for (k = 0; n != 1; n >>= 1) ++k; return k; }
static void __fastcall IL(E*, E*, E*, long d, Cmp) { g_i += d; }
static void __fastcall IS(E*, E*, Cmp) { ++g_f; }
static void __fastcall UA(E*, E*, E*, Cmp) { g_f += 10; }
static void __fastcall FS(E* f, E* l, Cmp c) { if (l - f > 16) { IS(f, f + 16, c); UA(f + 16, l, (E*)0, c); } else IS(f, l, c); }
static void __fastcall So(E* f, E* l, Cmp c) { if (f != l) { IL(f, l, (E*)0, Lg(l - f) * 2, c); FS(f, l, c); } }
int main()
{
    E a[20]; Cmp c = { 0 }; So(a, a + 20, c); if (g_i != 8 || g_f != 11) { std::printf("008298f0_TEST FAIL\n"); return 1; }
    So(a, a + 3, c); if (g_i != 10 || g_f != 12) { std::printf("008298f0_TEST FAIL\n"); return 1; }

    std::printf("008298f0_TEST PASS\n");
    return 0;
}

#include <cstdio>
static int g_f = 0; struct E { unsigned char b[12]; }; struct Cmp { void* c; };
static void __fastcall IS(E*, E*, Cmp) { ++g_f; }
static void __fastcall UA(E*, E*, E*, Cmp) { g_f += 10; }
static void __fastcall FS(E* f, E* l, Cmp c) { if (l - f > 16) { IS(f, f + 16, c); UA(f + 16, l, (E*)0, c); } else IS(f, l, c); }
int main()
{
    E a[20]; Cmp c = { 0 }; FS(a, a + 20, c); FS(a, a + 3, c); if (g_f != 12) { std::printf("00a418f0_TEST FAIL\n"); return 1; }

    std::printf("00a418f0_TEST PASS\n");
    return 0;
}
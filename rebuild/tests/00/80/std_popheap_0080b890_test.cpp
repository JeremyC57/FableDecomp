#include <cstdio>
// Behaviour model: the forwarder hands the callee the same range and an equal copy of the comparator.
struct Cmp { void* hero; float f[11]; bool b; float pref; };
static const void* g_first; static Cmp g_got;
static void aux(int* first, int* last, Cmp c) { g_first = first; g_got = c; (void)last; }
static void fwd(int* first, int* last, Cmp c) { aux(first, last, c); }
int main()
{
    int a[3]; Cmp c; c.hero = a; for (int i = 0; i < 11; ++i) c.f[i] = (float)i; c.b = true; c.pref = 2.5f;
    fwd(a, a + 3, c);
    if (g_first != a || g_got.hero != a || g_got.f[10] != 10.0f || !g_got.b || g_got.pref != 2.5f) { std::printf("0080b890_TEST FAIL\n"); return 1; }
    std::printf("0080b890_TEST PASS\n");
    return 0;
}
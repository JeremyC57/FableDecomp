#include <cstdio>
struct Fc; static Fc* g_o = 0; static void* g_n = 0; static int g_tag = 0;
struct Ag { unsigned char b[12]; Ag(Fc* o, void* n) { g_o = o; g_n = n; } };
struct Fc { bool Res(void* n, void** out) { if (!n) return false; *out = &g_tag; return true; } Ag* Cr(void* n) { if (!Res(n, &n)) return 0; return new Ag(this, n); } };
static bool Check() { Fc f; if (f.Cr(0) != 0) return false; int a; Ag* g = f.Cr(&a); bool ok = g && g_o == &f && g_n == &g_tag; delete g; return ok; }

int main()
{
    if (!Check()) { std::printf("00c1abb0_TEST FAIL\n"); return 1; }
    std::printf("01212379_TEST PASS\n");
    return 0;
}
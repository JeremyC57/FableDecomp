#include <cstdio>
static int g_e = 0, g_i = 0; struct E { unsigned char b[56]; };
struct V { E* f; E* l; E* e; E* Er(E* a, E*) { ++g_e; l = a; return a; } void In(E*, unsigned long c, const E&) { g_i += (int)c; } long size() const { return l - f; } void R(unsigned long n, const E& v) { if (n < (unsigned long)size()) Er(f + n, l); else In(l, n - size(), v); } };
int main()
{
    E buf[4]; V v; v.f = buf; v.l = buf + 4; v.e = buf + 4; E x; v.R(2, x); if (g_e != 1 || v.l != buf + 2) { std::printf("00f35450_TEST FAIL\n"); return 1; }
    v.R(5, x); if (g_i != 3) { std::printf("00f35450_TEST FAIL\n"); return 1; }

    std::printf("00f35450_TEST PASS\n");
    return 0;
}
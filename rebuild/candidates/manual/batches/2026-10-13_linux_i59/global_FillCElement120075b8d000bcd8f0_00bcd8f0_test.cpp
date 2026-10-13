#include <cstdio>
static int g_a = 0; struct E { unsigned char b[12]; E& operator=(const E& o) { ++g_a; b[0] = o.b[0]; return *this; } };
static void __fastcall F(E* f, E* l, const E& v) { for (; f != l; ++f) *f = v; }
int main()
{
    E a[2], v; v.b[0] = 7; F(a, a + 2, v); if (g_a != 2 || a[1].b[0] != 7) { std::printf("00bcd8f0_TEST FAIL\n"); return 1; }

    std::printf("00bcd8f0_TEST PASS\n");
    return 0;
}

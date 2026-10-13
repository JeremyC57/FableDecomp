#include <cstdio>
struct E { unsigned char b[8]; };
static void __fastcall F(E* f, E* l, const E& v) { for (; f != l; ++f) *f = v; }
int main()
{
    E a[2], v; v.b[0] = 7; F(a, a + 2, v); if (a[1].b[0] != 7) { std::printf("009b15a0_TEST FAIL\n"); return 1; }

    std::printf("009b15a0_TEST PASS\n");
    return 0;
}
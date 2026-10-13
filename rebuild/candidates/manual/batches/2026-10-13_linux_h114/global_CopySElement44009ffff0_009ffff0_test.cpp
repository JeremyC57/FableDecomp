#include <cstdio>
struct E { unsigned char b[44]; }; struct Tg { Tg() {} };
static E* __fastcall C(E* f, E* l, E* r, Tg, long*) { for (long n = l - f; n > 0; --n) { *r = *f; ++f; ++r; } return r; }
int main()
{
    E s[2], d[2]; s[0].b[0] = 1; s[1].b[0] = 2; d[0].b[0] = 0; d[1].b[0] = 0; E* e = C(s, s + 2, d, Tg(), 0); if (e != d + 2 || d[0].b[0] != 1 || d[1].b[0] != 2) { std::printf("009ffff0_TEST FAIL\n"); return 1; }

    std::printf("009ffff0_TEST PASS\n");
    return 0;
}

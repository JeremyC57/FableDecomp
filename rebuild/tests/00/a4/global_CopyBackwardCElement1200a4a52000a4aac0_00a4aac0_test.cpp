#include <cstdio>
struct E { unsigned char b[12]; E& operator=(const E& o) { b[0] = o.b[0]; return *this; } }; struct Tg { Tg() {} };
static E* __fastcall C(E* f, E* l, E* r, Tg, long*) { for (long n = l - f; n > 0; --n) *--r = *--l; return r; }
int main()
{
    E s[3]; s[0].b[0] = 1; s[1].b[0] = 2; s[2].b[0] = 9; E* e = C(s, s + 2, s + 3, Tg(), 0); if (e != s + 1 || s[1].b[0] != 1 || s[2].b[0] != 2) { std::printf("00a4aac0_TEST FAIL\n"); return 1; }

    std::printf("00a4aac0_TEST PASS\n");
    return 0;
}
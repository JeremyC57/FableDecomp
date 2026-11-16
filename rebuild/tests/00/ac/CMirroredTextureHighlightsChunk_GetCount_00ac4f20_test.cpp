#include <cstdio>
struct E { unsigned char b[4]; }; struct X { unsigned char p[4]; E* f; E* l; long C() { return l - f; } };
int main()
{
    E v[5]; X x; x.f = v; x.l = v + 5; if (x.C() != 5) { std::printf("00ac4f20_TEST FAIL\n"); return 1; }

    std::printf("00ac4f20_TEST PASS\n");
    return 0;
}
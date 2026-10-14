#include <cstdio>
struct R { long f; long l; }; struct X { unsigned char p[4]; R* r; long C() { return (r->l - r->f) / 20; } };
int main()
{
    R r; r.f = 0; r.l = 60; X x; x.r = &r; if (x.C() != 3) { std::printf("005c0923_TEST FAIL\n"); return 1; }

    std::printf("005c0923_TEST PASS\n");
    return 0;
}
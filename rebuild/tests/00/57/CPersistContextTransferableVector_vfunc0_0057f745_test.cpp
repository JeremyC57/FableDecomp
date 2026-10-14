#include <cstdio>
struct R { long f; long l; }; struct X { unsigned char p[4]; R* r; long C() { return (r->l - r->f) / 24; } };
int main()
{
    R r; r.f = 0; r.l = 72; X x; x.r = &r; if (x.C() != 3) { std::printf("0057f745_TEST FAIL\n"); return 1; }

    std::printf("0057f745_TEST PASS\n");
    return 0;
}
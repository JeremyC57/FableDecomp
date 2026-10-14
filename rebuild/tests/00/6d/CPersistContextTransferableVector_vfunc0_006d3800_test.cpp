#include <cstdio>
struct E { unsigned char b[8]; }; struct R { E* f; E* l; }; struct X { unsigned char p[4]; R* r; long C() { return r->l - r->f; } };
int main()
{
    E v[3]; R r; r.f = v; r.l = v + 3; X x; x.r = &r; if (x.C() != 3) { std::printf("006d3800_TEST FAIL\n"); return 1; }

    std::printf("006d3800_TEST PASS\n");
    return 0;
}
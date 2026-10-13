#include <cstdio>
static int g_t = 0; struct C { void A(long* v) { *v = 1; ++g_t; } void B(long* v) { *v = 2; ++g_t; } };
struct P { virtual void v00() {} long a, b, c; virtual void T(C& c) { c.A(&a); c.A(&b); c.B(&this->c); } };
int main()
{
    C c; P p; p.T(c); if (g_t != 3 || p.a != 1 || p.c != 2) { std::printf("004df562_TEST FAIL\n"); return 1; }

    std::printf("004df562_TEST PASS\n");
    return 0;
}

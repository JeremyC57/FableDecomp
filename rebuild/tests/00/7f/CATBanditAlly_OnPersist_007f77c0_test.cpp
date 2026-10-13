#include <cstdio>
static int g_t = 0; struct Ctx { void A(const char* n, long& v, const long& d) { v = n[0] + d; ++g_t; } void B(const char* n, signed char& v, const signed char& d) { v = (signed char)(n[0] + d); ++g_t; } };
struct P { unsigned char pad[0x20]; long a; signed char b; void F(Ctx* c, long) { c->A("A", a, 0); c->B("B", b, 0); } };
int main()
{
    Ctx c; P p; p.a = 0; p.b = 0; p.F(&c, 0); if (g_t != 2 || p.a != 'A' || p.b != 'B') { std::printf("007f77c0_TEST FAIL\n"); return 1; }

    std::printf("007f77c0_TEST PASS\n");
    return 0;
}
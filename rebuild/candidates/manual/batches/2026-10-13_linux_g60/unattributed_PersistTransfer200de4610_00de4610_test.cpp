#include <cstdio>
static int g_t = 0; struct Ctx { void Transfer(const char* n, signed char& v, const signed char& d) { v = (signed char)(n[0] + d); ++g_t; } };
struct P { virtual void v00() {} unsigned char pad[0x3d]; signed char a; signed char b; virtual void vf(Ctx* c) { c->Transfer("A", a, 0); c->Transfer("B", b, 0); } };
int main()
{
    Ctx c; P p; p.vf(&c); if (g_t != 2 || p.a != 'A' || p.b != 'B') { std::printf("00de4610_TEST FAIL\n"); return 1; }

    std::printf("00de4610_TEST PASS\n");
    return 0;
}

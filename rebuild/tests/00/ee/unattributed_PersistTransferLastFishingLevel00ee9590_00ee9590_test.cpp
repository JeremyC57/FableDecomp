#include <cstdio>
static int g_t = 0; struct Ctx { void Transfer(const char* n, long& v, const long& d) { v = n[0] + d + 1; ++g_t; } };
struct P { unsigned char pad[0x18]; long b; virtual void v00() {} virtual void vf(Ctx* c, long) { long d = 0; c->Transfer("A", b, d); } };
int main()
{
    Ctx c; P p; p.b = 0; p.vf(&c, 0); if (g_t != 1 || p.b != 'B') { std::printf("00ee9590_TEST FAIL\n"); return 1; }

    std::printf("00ee9590_TEST PASS\n");
    return 0;
}
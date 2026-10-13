#include <cstdio>
#include <new>
static int g_g = 0; struct Tg { Tg() {} };
struct V { long* f; long* l; long* e; void G(long*, const long&, const Tg&, unsigned long, bool) { ++g_g; } void P(const long& v) { if (l != e) { new (l) long(v); ++l; } else G(l, v, Tg(), 1, true); } };
int main()
{
    long buf[2]; V v; v.f = buf; v.l = buf; v.e = buf + 2; long x = 9; v.P(x); v.P(x); v.P(x); if (g_g != 1 || v.l != buf + 2 || buf[1] != 9) { std::printf("0049c450_TEST FAIL\n"); return 1; }

    std::printf("0049c450_TEST PASS\n");
    return 0;
}

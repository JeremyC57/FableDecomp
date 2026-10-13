#include <cstdio>
#include <new>
static int g_g = 0; struct Tg { Tg() {} };
struct V { long* f; long* l; long* e; void G(long*, const long&, const Tg&, unsigned long, bool) { ++g_g; } void P(const long& v) { if (l != e) { new (l) long(v); ++l; } else G(l, v, Tg(), 1, true); } };
static void __stdcall PC(V* v) { long value = 0x24; v->P(value); }
int main()
{
    long buf[1]; V v; v.f = buf; v.l = buf; v.e = buf + 1; PC(&v); PC(&v); if (g_g != 1 || buf[0] != 0x24 || v.l != buf + 1) { std::printf("00bcc7c0_TEST FAIL\n"); return 1; }

    std::printf("00bcc7c0_TEST PASS\n");
    return 0;
}

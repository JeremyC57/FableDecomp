#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct Tb { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual unsigned long RowLen(unsigned long r) { return 3; } virtual unsigned long Rows() { return 2; } virtual long Entry(unsigned long r, unsigned long c) { return (long)(r * 10 + c); } }; static Tb g_tb; struct St { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual Tb* Table() { return &g_tb; } };
struct X { long Q(St* s, unsigned long r, unsigned long c) { L l(&g_g); if (r >= s->Table()->Rows()) return 0; if (c >= s->Table()->RowLen(r)) return 0; return s->Table()->Entry(r, c); } };
static bool Check() { St s; X x; return x.Q(&s, 2, 0) == 0 && x.Q(&s, 1, 3) == 0 && x.Q(&s, 1, 2) == 12 && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c19760_TEST FAIL\n"); return 1; }
    std::printf("00c19760_TEST PASS\n");
    return 0;
}

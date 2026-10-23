#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct Hr { virtual void a() {} virtual void b() {} virtual void c() {} virtual void d() {} virtual void e() {} virtual Hr* G() { return this; } }; struct Q { long G(Hr* h) { L l(&g_g); return (long)h->G()->G(); } };
static bool Check() { Hr h; Q q; return q.G(&h) == (long)&h && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c19850_TEST FAIL\n"); return 1; }
    std::printf("01212124_TEST PASS\n");
    return 0;
}
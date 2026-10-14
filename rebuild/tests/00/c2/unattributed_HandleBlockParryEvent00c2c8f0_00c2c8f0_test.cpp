#include <cstdio>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static int g_a = 0, g_b = 0, g_d = 0; struct Ev { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual ~Ev() { ++g_d; } }; struct A { void H(Ev* e) { ++g_a; } }; struct B { void H(Ev* e) { ++g_b; } }; struct Ow { unsigned char p[8]; A* a; B* b; };
struct X { unsigned char p[4]; Ow* o; void HB(Ev* e) { L l(&g_g); o->a->H(e); o->b->H(e); delete e; } };
static bool Check() { A a; B b; Ow o; o.a = &a; o.b = &b; X x; x.o = &o; x.HB(new Ev); x.HB(0); return g_a == 2 && g_b == 2 && g_d == 1 && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c2c8f0_TEST FAIL\n"); return 1; }
    std::printf("00c2c8f0_TEST PASS\n");
    return 0;
}
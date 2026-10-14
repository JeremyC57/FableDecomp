#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; static G g_g2; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static int g_o = 0, g_h = 0, g_m = 0, g_rel = 0, g_del = 0; struct Pl { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void Release() { ++g_rel; } }; struct Ev { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual ~Ev() { ++g_del; } Pl* pl; };
struct Op { void H(Ev* e) { ++g_o; } }; struct Hd { void H(Ev* e) { ++g_h; } }; struct Mh { unsigned char b[0x28]; void H(Ev* e) { ++g_m; } };
struct X { unsigned char p[0x38]; Hd* h; Mh mh; Op* op; void Pr(Ev* e) { L l(&g_g); if (op) op->H(e); h->H(e); mh.H(e); if (e->pl) { e->pl->Release(); e->pl = 0; } delete e; } };
static bool Check() { Pl pl; Hd h; Op op; X x; x.h = &h; x.op = 0; Ev* e = new Ev; e->pl = &pl; x.Pr(e); x.op = &op; Ev* f = new Ev; f->pl = 0; x.Pr(f); return g_o == 1 && g_h == 2 && g_m == 2 && g_rel == 1 && g_del == 2 && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c08940_TEST FAIL\n"); return 1; }
    std::printf("00c08940_TEST PASS\n");
    return 0;
}
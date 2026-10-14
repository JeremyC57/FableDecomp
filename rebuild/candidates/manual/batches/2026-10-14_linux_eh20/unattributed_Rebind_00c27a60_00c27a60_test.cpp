#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; static G g_g2; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct X; static int g_rem = 0, g_add = 0, g_fc = 0, g_f8 = 0, g_v = 0; static void FreeC(void* b) { ++g_fc; } static void Free8(void* b) { ++g_f8; }
struct Rg { void Remove(X* i) { ++g_rem; } void Add(X* i) { ++g_add; } }; struct Ow { unsigned char p[0x2c]; Rg r; };
struct X { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void OnRebind() { ++g_v; } void* f; void* b8; void* bc; void Rebind(Ow* o) { o->r.Remove(this); OnRebind(); L l(&g_g2); FreeC(bc); Free8(b8); bc = 0; b8 = 0; f = 0; o->r.Add(this); } };
static bool Check() { Ow o; X x; int a; x.f = &a; x.b8 = &a; x.bc = &a; x.Rebind(&o); return g_rem == 1 && g_add == 1 && g_fc == 1 && g_f8 == 1 && g_v == 1 && !x.f && !x.b8 && !x.bc && g_l == 0; }

int main()
{
    if (!Check()) { std::printf("00c27a60_TEST FAIL\n"); return 1; }
    std::printf("00c27a60_TEST PASS\n");
    return 0;
}

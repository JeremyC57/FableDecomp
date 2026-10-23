#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct X; static int g_reset = 0; static bool g_acc = false; struct Ob { virtual void v0() {} virtual void Reset(X* s) { ++g_reset; } virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual void v6() {} virtual void v7() {} virtual void v8() {} virtual void v9() {} virtual long GetValue(X* s) { return 42; } virtual bool Accepts(X* s) { return g_acc; } }; static Ob g_ob; struct Obs { long a; Ob* Find(void* k) { return k ? &g_ob : 0; } };
struct X { unsigned char p[0x2c]; Obs obs; long Q(void* k) { L l(&g_g); Ob* o = obs.Find(k); if (o) { if (!o->Accepts(this)) { o->Reset(this); return -1; } return o->GetValue(this); } return -1; } };
static bool Check() { X* x = new X; int a; bool ok = x->Q(0) == -1 && g_reset == 0 && x->Q(&a) == -1 && g_reset == 1; g_acc = true; ok = ok && x->Q(&a) == 42 && g_reset == 1 && g_l == 0; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c11c00_TEST FAIL\n"); return 1; }
    std::printf("0121161c_TEST PASS\n");
    return 0;
}
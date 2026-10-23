#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
struct X; struct Ob { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual void v6() {} virtual void v7() {} virtual void v8() {} virtual float GetValue(X* s, float* out) { *out = 2.0f; return 3.5f; } }; static Ob g_ob; struct Obs { long a; Ob* Find(void* k) { return k ? &g_ob : 0; } };
struct X { unsigned char p[0x2c]; Obs obs; float Get(void* k, float* out) { L l(&g_g); Ob* o = obs.Find(k); if (!o) { *out = 0; return 1.0f; } return o->GetValue(this, out); } };
static bool Check() { X* x = new X; float f = 7; int a; bool ok = x->Get(0, &f) == 1.0f && f == 0 && x->Get(&a, &f) == 3.5f && f == 2.0f && g_l == 0; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c11650_TEST FAIL\n"); return 1; }
    std::printf("01211572_TEST PASS\n");
    return 0;
}
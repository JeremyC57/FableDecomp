#include <cstdio>
#include <new>
struct X; static X* g_o = 0; static float g_r = 0; static void* g_t = 0; static float g_sc = 0; struct Tb { unsigned char b[0x30]; Tb(X* o) { g_o = o; } ~Tb() {} }; struct Tg { virtual void v0() {} Tb* t; float s; Tg(Tb* T, float S) : t(T), s(S) {} }; struct Gf { float v; void Bind(float r, Tg* t) { g_r = r; g_t = t->t; g_sc = t->s; } };
struct X { virtual void v0() {} Tb tr; float wv; float val; X(float v, Gf w) : tr(this) { wv = w.v; val = v; Tg t(&tr, w.v); w.Bind(v / w.v, &t); } };
static bool Check() { Gf w; w.v = 2.0f; X x(6.0f, w); return g_o == &x && g_r == 3.0f && g_t == &x.tr && g_sc == 2.0f && x.wv == 2.0f && x.val == 6.0f; }

int main()
{
    if (!Check()) { std::printf("00c467d0_TEST FAIL\n"); return 1; }
    std::printf("01215575_TEST PASS\n");
    return 0;
}
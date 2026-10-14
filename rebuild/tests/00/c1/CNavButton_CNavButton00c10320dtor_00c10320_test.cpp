#include <cstdio>
#include <new>
static int g_det = 0, g_rel = 0, g_nd = 0; struct En { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual void v6() {} virtual void Detach(void* o) { ++g_det; } virtual void Release(long f) { ++g_rel; } };
static En g_e[4]; struct Nd { unsigned char b[8]; ~Nd() { ++g_nd; } }; struct Rt { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual void v6() {} virtual void v7() {} virtual void v8() {} virtual void v9() {} virtual void v10() {} virtual En* First() { return n > 0 ? &g_e[--n] : 0; } Nd* pn; long n; ~Rt() {} };
struct Ca : Rt { Nd nd; ~Ca() {} }; struct Cb : Rt { Nd nd; ~Cb() {} }; struct X : Ca { Cb m; En* FE() { return First(); } virtual ~X() { En* e; for (e = m.First(); e; e = m.First()) { e->Detach(this); e->Release(1); } for (e = FE(); e; e = FE()) { e->Detach(this); e->Release(1); } } };
static bool Check() { X* x = new X; x->n = 2; x->m.n = 3; delete x; return g_det == 5 && g_rel == 5 && g_nd == 2; }

int main()
{
    if (!Check()) { std::printf("00c10320_TEST FAIL\n"); return 1; }
    std::printf("00c10320_TEST PASS\n");
    return 0;
}
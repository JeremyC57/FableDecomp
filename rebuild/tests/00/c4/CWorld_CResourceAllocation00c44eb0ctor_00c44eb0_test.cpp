#include <cstdio>
#include <new>
static int g_p8 = 0, g_p20 = 0, g_arr = 0, g_reg = 0, g_i1 = 0, g_i2 = 0; static long g_v8 = 0, g_cnt = 0; static void* g_regd = 0;
struct Nd { unsigned char b[0x28]; Nd(long m) {} ~Nd() {} }; struct Rt { virtual void v0() {} Nd* pn; Rt(Nd* n) : pn(n) {} ~Rt() {} }; struct Ct : Rt { Nd n; Ct() : Rt(&n), n(0) {} ~Ct() {} };
struct P8 { unsigned char b[0x18]; P8(long v) { ++g_p8; g_v8 = v; } ~P8() {} }; struct P20 { unsigned char b[0x30]; P20(P8* p) { ++g_p20; } ~P20() {} }; struct Ar { unsigned char b[0xc]; Ar(long n) { ++g_arr; g_cnt = n; } ~Ar() {} };
struct Op { long Get() { return 77; } }; struct Ow { unsigned char p[8]; Op* part; }; struct Rs { long Lookup(long k) { return k * 2; } };
struct Rg { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual void v6() {} virtual void v7() {} virtual void v8() {} virtual void Reg(void* e) { ++g_reg; g_regd = e; } }; struct Wd { Rs* res; unsigned char p[0x10]; Rg reg; unsigned char q[0x164 - 0x18]; long key; };
struct X { Wd* w; Ow* o; P8 m8; P20 m20; Ct m50; Ar m80; void I1() { ++g_i1; } void I2() { ++g_i2; } X(Wd* W, Ow* O) : w(W), o(O), m8(O->part->Get()), m20(&m8), m50(), m80(w->res->Lookup(w->key)) { w->reg.Reg(this); I1(); I2(); } };
static bool Check() { Wd w; Rs rs; w.res = &rs; w.key = 21; Op op; Ow o; o.part = &op; X* x = new X(&w, &o); bool ok = x->w == &w && x->o == &o && g_p8 == 1 && g_v8 == 77 && g_p20 == 1 && g_arr == 1 && g_cnt == 42 && g_reg == 1 && g_regd == x && g_i1 == 1 && g_i2 == 1 && x->m50.pn == &x->m50.n; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c44eb0_TEST FAIL\n"); return 1; }
    std::printf("00c44eb0_TEST PASS\n");
    return 0;
}
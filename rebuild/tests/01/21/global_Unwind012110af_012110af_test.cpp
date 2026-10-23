#include <cstdio>
#include <new>
#include <cstdlib>
static int g_s1 = 0, g_s2 = 0, g_s3 = 0, g_s4 = 0, g_rem = 0, g_nav = 0, g_sn = 0, g_bn = 0; struct X; struct Rg { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual void v6() {} virtual void v7() {} virtual void Remove(X* d) { ++g_rem; } virtual void Add(X* d) {} }; struct Ow { unsigned char p[4]; Rg r; };
struct Bs { virtual void v0() {} ~Bs() {} }; struct Nv { unsigned char b[0x20]; ~Nv() { ++g_nav; } }; struct Bf { void* p; long a, b; ~Bf() { if (p) { std::free(p); p = 0; } } }; struct Sn { unsigned char b[8]; ~Sn() { ++g_sn; } }; struct Bn { unsigned char b[0x28]; ~Bn() { ++g_bn; } };
struct Ra { virtual void v0() {} Sn* pn; ~Ra() {} }; struct Rb { virtual void v0() {} Sn* pn; ~Rb() {} }; struct Rc { virtual void v0() {} Sn* pn; ~Rc() {} }; struct Rd { virtual void v0() {} Bn* pn; ~Rd() {} }; struct Ca : Ra { Sn n; ~Ca() {} }; struct Cb : Rb { Sn n; ~Cb() {} }; struct Cc : Rc { Sn n; ~Cc() {} }; struct Cd : Rd { Bn n; ~Cd() {} };
struct X : Bs { long m4; Ow* o; Nv nv; Bf bf; Ca a; long m48, m4c; Cb b; Cc c; long m70; Cd d; void S1() { ++g_s1; } void S2() { ++g_s2; } void S3() { ++g_s3; } void S4() { ++g_s4; } ~X() { S1(); S2(); S3(); S4(); o->r.Remove(this); } };
static bool Check() { Ow o; X* x = new X; x->o = &o; x->bf.p = std::malloc(8); delete x; return g_s1 == 1 && g_s2 == 1 && g_s3 == 1 && g_s4 == 1 && g_rem == 1 && g_nav == 1 && g_sn == 3 && g_bn == 1; }

int main()
{
    if (!Check()) { std::printf("00c0ece0_TEST FAIL\n"); return 1; }
    std::printf("012110af_TEST PASS\n");
    return 0;
}
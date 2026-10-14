#include <cstdio>
#include <new>
static int g_ls = 0; struct Ls { unsigned char b[0x14]; Ls() { ++g_ls; } ~Ls() {} }; struct Bs { virtual void v0() {} Bs() {} ~Bs() {} };
struct X : Bs { long m4, m8, mc; unsigned char p[0xc]; Ls l1, l2, l3, l4; long m6c, m70, m74, m78, m7c, m80, m84, m88; float m8c, m90, m94, m98, m9c, ma0; long ma4;
  X() : l1(), l2(), l3(), l4() { m6c = -1; m70 = -1; m74 = -1; m4 = 0; m8 = 0; mc = 0; ma4 = 0; m80 = 0; m84 = 0; m7c = 0; m8c = 1.0f; m90 = 1.0f; m94 = 1.0f; m9c = 1.0f; ma0 = 1.0f; m78 = 1; m88 = 5000; m98 = 1.0f / 330.0f; } };
static bool Check() { X* x = new X; bool ok = g_ls == 4 && x->m6c == -1 && x->m74 == -1 && x->m4 == 0 && x->ma4 == 0 && x->m8c == 1.0f && x->ma0 == 1.0f && x->m78 == 1 && x->m88 == 5000 && x->m98 > 0.00303f && x->m98 < 0.00304f; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c3d7d0_TEST FAIL\n"); return 1; }
    std::printf("00c3d7d0_TEST PASS\n");
    return 0;
}
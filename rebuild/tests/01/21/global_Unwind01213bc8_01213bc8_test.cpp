#include <cstdio>
#include <new>
static int g_next = 2, g_rem = 0, g_str = 0, g_node = 0, g_base = 0; struct Bs { virtual void v0() {} unsigned char b[0x78]; ~Bs() { ++g_base; } }; struct Mk { long v; ~Mk() {} }; struct St { void* d; ~St() { ++g_str; } }; struct It { long k; long m4; St name; }; struct Nd { unsigned char b[0x28]; ~Nd() { ++g_node; } };
struct Cr { virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {} virtual void v4() {} virtual void v5() {} virtual void v6() {} virtual void v7() {} virtual void Remove(It* i) { ++g_rem; } virtual void v9() {} virtual void v10() {} virtual It* Next() { return g_next-- > 0 ? new It : 0; } Nd* pn; ~Cr() {} }; struct Ct : Cr { Nd n; ~Ct() {} };
struct X : Bs { Mk mk; Ct items; ~X() { It* i = items.Next(); while (i) { items.Remove(i); delete i; i = items.Next(); } } };
static bool Check() { { X x; } return g_rem == 2 && g_str == 2 && g_node == 1 && g_base == 1; }

int main()
{
    if (!Check()) { std::printf("00c2dbe0_TEST FAIL\n"); return 1; }
    std::printf("01213bc8_TEST PASS\n");
    return 0;
}
// Behaviour test for 00caef70: the vcall thunk (??_9@$BDA@AG) instantiated by a pointer to the __stdcall virtual slot 12 of
// IComObject_Retail, exercised through the member pointer on two implementations (the thunk forwards the stack `this`).
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct IComObject_Retail {
    virtual long __stdcall v00() { return 0; }
    virtual long __stdcall v01() { return 1; }
    virtual long __stdcall v02() { return 2; }
    virtual long __stdcall v03() { return 3; }
    virtual long __stdcall v04() { return 4; }
    virtual long __stdcall v05() { return 5; }
    virtual long __stdcall v06() { return 6; }
    virtual long __stdcall v07() { return 7; }
    virtual long __stdcall v08() { return 8; }
    virtual long __stdcall v09() { return 9; }
    virtual long __stdcall v10() { return 10; }
    virtual long __stdcall v11() { return 11; }
    virtual long __stdcall vfunc_12() { return 12; }
};
typedef long (__stdcall IComObject_Retail::*PFN_IComObject_vfunc_12)();
PFN_IComObject_vfunc_12 g_pfnIComObject_vfunc_12 = &IComObject_Retail::vfunc_12;
static int g_calls = 0; static IComObject_Retail* g_lastThis = 0;
struct CImpl : IComObject_Retail {
    long Value;
    CImpl(long v) : Value(v) {}
    virtual long __stdcall vfunc_12() { ++g_calls; g_lastThis = this; return Value; }
};
int main()
{
    IComObject_Retail base;
    CHECK((base.*g_pfnIComObject_vfunc_12)() == 12);            // the base slot through the thunk
    CImpl a(100), b(-7);
    IComObject_Retail* pa = &a; IComObject_Retail* pb = &b;
    CHECK((pa->*g_pfnIComObject_vfunc_12)() == 100); CHECK(g_calls == 1 && g_lastThis == pa);
    CHECK((pb->*g_pfnIComObject_vfunc_12)() == -7);  CHECK(g_calls == 2 && g_lastThis == pb);
    CHECK((pa->*g_pfnIComObject_vfunc_12)() == 100); CHECK(g_calls == 3);
    if (fails == 0) printf("00caef70_TEST PASS\n"); else printf("00caef70_TEST FAIL\n");
    return fails;
}
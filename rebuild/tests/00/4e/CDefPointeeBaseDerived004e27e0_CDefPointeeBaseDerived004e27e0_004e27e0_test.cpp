#include <stdio.h>
#include <stddef.h>
static char g_log[16]; static int g_n;
struct CContainer_004dd42f { unsigned char _body[0x0c]; ~CContainer_004dd42f() { g_log[g_n++] = (char)_body[0]; } };
struct CContainer_004dd2b1 { unsigned char _body[0x0c]; ~CContainer_004dd2b1() { g_log[g_n++] = (char)_body[0]; } };
struct CBase_009fc550 { virtual void v00() {} ~CBase_009fc550() { g_log[g_n++] = 'B'; } };
class CDefPointeeBase : public CBase_009fc550 { public: virtual void v00() {} ~CDefPointeeBase() { } };
struct __declspec(novtable) CDefPointeeBase_Derived_004e27e0 : CDefPointeeBase {
    unsigned char _pad0[0x28]; CContainer_004dd42f m_items0; CContainer_004dd2b1 m_items1; CContainer_004dd42f m_items2;
    virtual void v00() {}
    ~CDefPointeeBase_Derived_004e27e0();
};
CDefPointeeBase_Derived_004e27e0::~CDefPointeeBase_Derived_004e27e0() { }
int main()
{
    bool ok = offsetof(CDefPointeeBase_Derived_004e27e0, m_items0) == 0x2c && offsetof(CDefPointeeBase_Derived_004e27e0, m_items1) == 0x38 && offsetof(CDefPointeeBase_Derived_004e27e0, m_items2) == 0x44;
    { CDefPointeeBase_Derived_004e27e0 d; d.m_items0._body[0] = 'a'; d.m_items1._body[0] = 'b'; d.m_items2._body[0] = 'c'; }
    ok = ok && g_n == 4 && g_log[0] == 'c' && g_log[1] == 'b' && g_log[2] == 'a' && g_log[3] == 'B';
    if (!ok) { printf("004e27e0_TEST FAIL\n"); return 1; }
    printf("004e27e0_TEST PASS\n"); return 0;
}
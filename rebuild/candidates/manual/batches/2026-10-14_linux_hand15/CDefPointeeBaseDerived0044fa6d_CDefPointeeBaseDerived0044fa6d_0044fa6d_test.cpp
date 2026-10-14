#include <stdio.h>
#include <stddef.h>
static char g_log[16]; static int g_n;
struct CCharString { void* m_pStringData; ~CCharString() { g_log[g_n++] = (char)('0' + (int)((char*)this - (char*)m_pStringData)); } };
struct CBase_009fc550 { virtual void v00() {} ~CBase_009fc550() { g_log[g_n++] = 'B'; } };
struct CDefPointeeBase : CBase_009fc550 { virtual void v00() {} ~CDefPointeeBase() { } };
struct __declspec(novtable) CDefPointeeBase_Derived_0044fa6d : CDefPointeeBase {
    unsigned char _pad0[0x24]; CCharString m_string0; unsigned char _pad1[0x20]; CCharString m_string1; CCharString m_string2;
    virtual void v00() {}
    ~CDefPointeeBase_Derived_0044fa6d();
};
CDefPointeeBase_Derived_0044fa6d::~CDefPointeeBase_Derived_0044fa6d() { }
int main()
{
    bool ok = offsetof(CDefPointeeBase_Derived_0044fa6d, m_string0) == 0x28 && offsetof(CDefPointeeBase_Derived_0044fa6d, m_string1) == 0x4c && offsetof(CDefPointeeBase_Derived_0044fa6d, m_string2) == 0x50;
    {
        CDefPointeeBase_Derived_0044fa6d d;
        d.m_string0.m_pStringData = (char*)&d.m_string0 - 0; d.m_string1.m_pStringData = (char*)&d.m_string1 - 1; d.m_string2.m_pStringData = (char*)&d.m_string2 - 2;
    }
    ok = ok && g_n == 4 && g_log[0] == '2' && g_log[1] == '1' && g_log[2] == '0' && g_log[3] == 'B';
    if (!ok) { printf("0044fa6d_TEST FAIL\n"); return 1; }
    printf("0044fa6d_TEST PASS\n"); return 0;
}

#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __cdecl
#endif
static int g_assigns = 0;
struct CKeyVector { void* m_pFirst; void* m_pLast; void* m_pEnd;
    CKeyVector& operator=(const CKeyVector& o) { ++g_assigns; m_pFirst = o.m_pFirst; m_pLast = o.m_pLast; m_pEnd = o.m_pEnd; return *this; } };
struct CSplineData_00b20b60 {
    long       m_head;
    bool       m_flagA;
    bool       m_flagB;
    CKeyVector m_keysA;
    CKeyVector m_keysB;
    void CopyFrom(const CSplineData_00b20b60* other)
    {
        m_flagB = other->m_flagB;
        m_flagA = other->m_flagA;
        m_keysA = other->m_keysA;
        m_keysB = other->m_keysB;
    }
};
int main()
{
    int a = 0, b = 0;
    CSplineData_00b20b60 src, dst;
    src.m_head = 1; src.m_flagA = true; src.m_flagB = false; src.m_keysA.m_pFirst = &a; src.m_keysA.m_pLast = 0; src.m_keysA.m_pEnd = 0;
    src.m_keysB.m_pFirst = &b; src.m_keysB.m_pLast = &b; src.m_keysB.m_pEnd = 0;
    dst.m_head = 9; dst.m_flagA = false; dst.m_flagB = true; dst.m_keysA.m_pFirst = 0; dst.m_keysB.m_pFirst = 0;
    dst.CopyFrom(&src);
    if (g_assigns != 2 || !dst.m_flagA || dst.m_flagB || dst.m_keysA.m_pFirst != &a || dst.m_keysB.m_pLast != &b || dst.m_head != 9) { std::printf("00b20b60_TEST FAIL\n"); return 1; }
    std::printf("00b20b60_TEST PASS\n");
    return 0;
}
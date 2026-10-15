#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __cdecl
#endif
struct CRangedObject {
    long          m_first;
    long          m_last;
    unsigned long m_flags;
    void SetRange(long first, long last) { m_first = first; m_last = last; m_flags |= 0x40000; }
};
int main()
{
    CRangedObject o; o.m_first = 0; o.m_last = 0; o.m_flags = 1;
    o.SetRange(-5, 12);
    if (o.m_first != -5 || o.m_last != 12 || o.m_flags != 0x40001) { std::printf("00c417a0_TEST FAIL\n"); return 1; }
    std::printf("00c417a0_TEST PASS\n");
    return 0;
}

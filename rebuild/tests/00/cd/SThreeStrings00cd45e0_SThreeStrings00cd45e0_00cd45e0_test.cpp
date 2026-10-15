#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
typedef long long Int64;
typedef unsigned long long UInt64;
#else
typedef __int64 Int64;
typedef unsigned __int64 UInt64;
#endif
// Behaviour test for 00cd45e0: the record's constructor default-constructs its three CCharString members in declaration order.
static int g_order = 0;
struct CCharString { long m_order; CCharString() : m_order(++g_order) {} };
struct SThreeStrings_00cd45e0 {
    CCharString m_first;
    CCharString m_second;
    CCharString m_third;
    SThreeStrings_00cd45e0();
};
SThreeStrings_00cd45e0::SThreeStrings_00cd45e0()
{
}
int main()
{
    SThreeStrings_00cd45e0 r;
    if (r.m_first.m_order != 1 || r.m_second.m_order != 2 || r.m_third.m_order != 3) { std::printf("00cd45e0_TEST FAIL\n"); return 1; }
    std::printf("00cd45e0_TEST PASS\n");
    return 0;
}
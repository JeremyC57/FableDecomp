// behaviour test for 0x00f37ad0 (std::__uninitialized_fill_n over 44-byte records)
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
static int g_stringCopies = 0, g_stringAssigns = 0;
#include <new>
struct SRecord44 {                              // 44 bytes, eleven scalar words
    unsigned long m_00, m_04, m_08, m_0c, m_10, m_14, m_18, m_1c, m_20, m_24, m_28;
    SRecord44(const SRecord44& o)
        : m_00(o.m_00), m_04(o.m_04), m_08(o.m_08), m_0c(o.m_0c), m_10(o.m_10), m_14(o.m_14), m_18(o.m_18),
          m_1c(o.m_1c), m_20(o.m_20), m_24(o.m_24), m_28(o.m_28) {}
};
struct __false_type { __false_type() {} };
namespace std {
template <class ForwardIter, class Size, class T>
ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) T(x);
    return cur;
}
template SRecord44* __fastcall __uninitialized_fill_n(SRecord44*, unsigned int, const SRecord44&, const __false_type&);
}

int main()
{
    static double raw[64];
    SRecord44* p = (SRecord44*)raw;
    p[3].m_00 = 77;
    static double xs[16];
    SRecord44& x = *(SRecord44*)xs;
    x.m_00 = 1; x.m_04 = 2; x.m_08 = 3; x.m_0c = 4; x.m_10 = 5; x.m_14 = 6; x.m_18 = 7; x.m_1c = 8; x.m_20 = 9; x.m_24 = 10; x.m_28 = 11;
    __false_type tag;
    SRecord44* e = std::__uninitialized_fill_n(p, 3u, x, tag);
    int ok = e == p + 3 && p[0].m_00 == 1 && p[1].m_14 == 6 && p[2].m_28 == 11 && p[3].m_00 == 77;
    if (!ok) { printf("00f37ad0_TEST FAIL\n"); return 1; }
    printf("00f37ad0_TEST PASS\n");
    return 0;
}
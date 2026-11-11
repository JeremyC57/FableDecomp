#include <cstdio>
#include <new>
// behaviour copy of the 28-byte record __uninitialized_fill_n @ 0x00BF5BB0
struct SRecord12 {
    long m_a, m_b, m_c;
    SRecord12(long a, long b, long c) : m_a(a), m_b(b), m_c(c) { }
    SRecord12(const SRecord12& o) : m_a(o.m_a), m_b(o.m_b), m_c(o.m_c) { }
};
struct SRecord28 {
    long      m_0, m_4, m_8;
    SRecord12 m_record;
    long      m_18;
    SRecord28(long base) : m_0(base), m_4(base + 1), m_8(base + 2), m_record(base + 3, base + 4, base + 5), m_18(base + 6) { }
    SRecord28(const SRecord28& o) : m_0(o.m_0), m_4(o.m_4), m_8(o.m_8), m_record(o.m_record), m_18(o.m_18) { }
};
struct __false_type { __false_type() { } };

SRecord28* UninitFillN(SRecord28* first, unsigned int n, const SRecord28& x, const __false_type&)
{
    SRecord28* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) SRecord28(x);
    return cur;
}

int main()
{
    static long raw[7 * 5];
    for (int i = 0; i < 35; ++i) raw[i] = -1;
    SRecord28* first = (SRecord28*)raw;
    SRecord28 value(100);
    int ok = 1;
    if (UninitFillN(first, 0, value, __false_type()) != first) ok = 0;
    if (raw[0] != -1) ok = 0;
    SRecord28* end = UninitFillN(first, 4, value, __false_type());
    if (end != first + 4) ok = 0;
    for (int i = 0; i < 4; ++i)
        if (first[i].m_0 != 100 || first[i].m_8 != 102 || first[i].m_record.m_b != 104 || first[i].m_18 != 106) ok = 0;
    if (raw[28] != -1) ok = 0;    // the fifth slot stays untouched
    if (!ok) { std::printf("00bf5bb0_TEST FAIL\n"); return 1; }
    std::printf("00bf5bb0_TEST PASS\n");
    return 0;
}
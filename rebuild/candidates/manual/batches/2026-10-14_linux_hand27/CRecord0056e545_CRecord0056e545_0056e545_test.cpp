#include <cstdio>
struct CRecord_0056e545 {
    long m_first, m_second, m_third, m_fourth; float m_time; long m_fifth; unsigned char m_flagA, m_flagB;
    CRecord_0056e545(long a, long b, long c, long d, long e) : m_first(a), m_second(b), m_third(c), m_fourth(d), m_time(0.0f), m_fifth(e), m_flagA(0), m_flagB(0) { }
};
int main()
{
    CRecord_0056e545 r(1, 2, 3, 4, 5);
    if (r.m_first != 1 || r.m_second != 2 || r.m_third != 3 || r.m_fourth != 4 || r.m_time != 0.0f || r.m_fifth != 5 || r.m_flagA || r.m_flagB || sizeof(r) != 28) { std::printf("0056e545_TEST FAIL\n"); return 1; }
    std::printf("0056e545_TEST PASS\n"); return 0;
}

#include <cstdio>
struct CRecord_006a92b0 {
    long m_first, m_second, m_third, m_fourth; unsigned char m_flag;
    CRecord_006a92b0(long a, long b, long c, long d, unsigned char f) : m_first(a), m_second(b), m_third(c), m_fourth(d), m_flag(f) { }
};
int main()
{
    CRecord_006a92b0 r(10, 20, 30, 40, 7);
    if (r.m_first != 10 || r.m_second != 20 || r.m_third != 30 || r.m_fourth != 40 || r.m_flag != 7 || sizeof(r) != 20) { std::printf("006a92b0_TEST FAIL\n"); return 1; }
    std::printf("006a92b0_TEST PASS\n"); return 0;
}

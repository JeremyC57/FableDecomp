#include <cstdio>
#include <cstring>
struct SRecord {
    long m_a, m_b, m_c, m_capacity, m_e, m_f;
    SRecord(long) : m_a(0), m_b(0), m_c(0), m_capacity(10), m_e(0), m_f(0) {}
};
int main()
{
    int ok = 1;
    SRecord r(1234);
    if (r.m_a || r.m_b || r.m_c || r.m_capacity != 10 || r.m_e || r.m_f) ok = 0;
    if (!ok) { std::printf("00cb12e0_TEST FAIL\n"); return 1; }
    std::printf("00cb12e0_TEST PASS\n");
    return 0;
}

#include <cstdio>
#include <cstring>
struct SRecord {
    long m_a, m_b, m_c, m_d, m_e, m_f;
    SRecord() : m_a(0), m_b(0), m_c(0), m_e(0), m_f(0) { m_d = 0; }
};
int main()
{
    int ok = 1;
    SRecord r;
    if (r.m_a || r.m_b || r.m_c || r.m_d || r.m_e || r.m_f) ok = 0;
    if (!ok) { std::printf("006815f0_TEST FAIL\n"); return 1; }
    std::printf("006815f0_TEST PASS\n");
    return 0;
}
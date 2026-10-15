#include <cstdio>
#include <cstring>
struct CQuestInfoElement {
    virtual long Kind() { return 3; }
    long m_a, m_b, m_value, m_d, m_e, m_f;
    CQuestInfoElement(long value) : m_a(0), m_b(0), m_value(value), m_d(0), m_e(0), m_f(0) {}
};
int main()
{
    int ok = 1;
    CQuestInfoElement e(42);
    if (e.m_a || e.m_b || e.m_value != 42 || e.m_d || e.m_e || e.m_f || e.Kind() != 3) ok = 0;
    if (!ok) { std::printf("00656795_TEST FAIL\n"); return 1; }
    std::printf("00656795_TEST PASS\n");
    return 0;
}
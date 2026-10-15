#include <cstdio>
#include <cstring>
struct SVtableRecord {
    virtual long Kind() { return 7; }
    long m_a, m_b, m_index, m_d;
    SVtableRecord() : m_a(0), m_b(0), m_index(-1), m_d(0) {}
};
int main()
{
    int ok = 1;
    SVtableRecord r;
    if (r.m_a || r.m_b || r.m_index != -1 || r.m_d || r.Kind() != 7) ok = 0;
    if (!ok) { std::printf("00c3c8a0_TEST FAIL\n"); return 1; }
    std::printf("00c3c8a0_TEST PASS\n");
    return 0;
}
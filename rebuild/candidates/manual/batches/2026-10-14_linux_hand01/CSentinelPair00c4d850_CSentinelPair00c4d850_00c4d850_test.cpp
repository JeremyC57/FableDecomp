#include <cstdio>
struct CSentinelPair { unsigned long m_first, m_second, m_count, m_extra; CSentinelPair() { m_count = m_extra = 0; m_first = m_second = 0xC0DAF00D; } };
int main()
{
    CSentinelPair p;
    if (sizeof(p) != 16 || p.m_first != 0xC0DAF00D || p.m_second != 0xC0DAF00D || p.m_count != 0 || p.m_extra != 0) { std::printf("00c4d850_TEST FAIL\n"); return 1; }
    std::printf("00c4d850_TEST PASS\n"); return 0;
}

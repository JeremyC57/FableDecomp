// Test for 0x00C3F000 CLimitedMixBuffer::Reset: every one of the six limiters is cleared.
#include <cstdio>
struct CLimiter { int m_delay[88]; unsigned int m_writeIndex; int m_state; int m_holdCount;
    void Clear() { for (int i = 0; i < 88; ++i) m_delay[i] = 0; m_writeIndex = 0; m_state = 0; } };
struct CLimitedMixBuffer { CLimiter m_limiters[6]; void Reset() { for (int i = 0; i < 6; ++i) m_limiters[i].Clear(); } };
int main()
{
    static CLimitedMixBuffer b;
    for (int i = 0; i < 6; ++i) { b.m_limiters[i].m_delay[5] = 9; b.m_limiters[i].m_writeIndex = 3; b.m_limiters[i].m_state = 2; b.m_limiters[i].m_holdCount = 7; }
    b.Reset();
    for (int i = 0; i < 6; ++i)
        if (b.m_limiters[i].m_delay[5] != 0 || b.m_limiters[i].m_writeIndex != 0 || b.m_limiters[i].m_state != 0 || b.m_limiters[i].m_holdCount != 7)
        { std::printf("00c3f000_TEST FAIL\n"); return 1; }
    std::printf("00c3f000_TEST PASS\n");
    return 0;
}
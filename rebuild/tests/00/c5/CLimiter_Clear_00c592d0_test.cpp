// Behaviour test for 00c592d0: CLimiter::Clear -- delay line, write index and state cleared; the rest kept
#include <cstdio>
#include <cstring>
class CLimiter {
public:
    virtual ~CLimiter() { }
    long m_delay[88]; unsigned long m_writeIndex; float m_peak; float m_envelope; long m_holdCount; long m_state; long m_releaseCount;
    void Clear() { std::memset(m_delay, 0, sizeof(m_delay)); m_writeIndex = 0; m_state = 0; }
};
int main()
{
    CLimiter l;
    for (int i = 0; i < 88; ++i) l.m_delay[i] = i + 1;
    l.m_writeIndex = 40; l.m_peak = 3.0f; l.m_envelope = 4.0f; l.m_holdCount = 7; l.m_state = 2; l.m_releaseCount = 9;
    l.Clear();
    int ok = l.m_writeIndex == 0 && l.m_state == 0 && l.m_peak == 3.0f && l.m_envelope == 4.0f && l.m_holdCount == 7 && l.m_releaseCount == 9;
    for (int i = 0; i < 88; ++i) ok = ok && l.m_delay[i] == 0;
    std::printf(ok ? "00c592d0_TEST PASS\n" : "00c592d0_TEST FAIL\n");
    return ok ? 0 : 1;
}

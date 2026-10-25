// Behaviour test for CLimiter::CLimiter (0x00C59570): the delay line, write index and state start cleared.
#include <cstdio>
#include <cstring>
#include <new>
struct CLimiter {
    int          m_delay[88];
    unsigned int m_writeIndex;
    float        m_peak;
    float        m_envelope;
    int          m_holdCount;
    int          m_state;
    int          m_releaseCount;
    CLimiter()
    {
        std::memset(m_delay, 0, sizeof(m_delay));
        m_writeIndex = 0;
        m_state = 0;
    }
};
int main()
{
    unsigned char raw[sizeof(CLimiter)];
    std::memset(raw, 0x5a, sizeof(raw));
    CLimiter* p = new (raw) CLimiter;
    int ok = p->m_writeIndex == 0 && p->m_state == 0;
    for (int i = 0; i < 88; ++i) ok = ok && p->m_delay[i] == 0;
    ok = ok && p->m_holdCount == 0x5a5a5a5a;                       // untouched by the constructor
    if (!ok) { std::printf("00c59570_TEST FAIL\n"); return 1; }
    std::printf("00c59570_TEST PASS\n");
    return 0;
}
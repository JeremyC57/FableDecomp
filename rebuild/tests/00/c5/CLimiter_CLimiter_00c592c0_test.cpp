// Behaviour test for 00c592c0: CLimiter::~CLimiter -- destroying a limiter touches none of its state
#include <cstdio>
#include <new>
class CLimiter {
public:
    virtual ~CLimiter() { }
    long m_delay[88]; unsigned long m_writeIndex; float m_peak; float m_envelope; long m_holdCount; long m_state; long m_releaseCount;
};
int main()
{
    void* raw = operator new(sizeof(CLimiter));
    CLimiter* l = new (raw) CLimiter; l->m_state = 2; l->m_writeIndex = 17; l->m_delay[5] = -3;
    l->~CLimiter();
    const CLimiter* c = (const CLimiter*)raw;
    int ok = sizeof(CLimiter) == 0x17c && c->m_state == 2 && c->m_writeIndex == 17 && c->m_delay[5] == -3;
    operator delete(raw);
    std::printf(ok ? "00c592c0_TEST PASS\n" : "00c592c0_TEST FAIL\n");
    return ok ? 0 : 1;
}

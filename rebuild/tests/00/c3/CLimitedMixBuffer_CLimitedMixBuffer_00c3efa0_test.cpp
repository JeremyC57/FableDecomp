// Behaviour test for 00c3efa0: CLimitedMixBuffer::CLimitedMixBuffer -- the base first, then six limiters in order
#include <cstdio>
#include <cstring>
static char g_seq[16] = ""; static int g_limCtor = 0;
class CMixBuffer {
public:
    CMixBuffer() : m_active(1), m_numChannels(0) { std::strcat(g_seq, "M"); }
    virtual ~CMixBuffer() { }
    long* m_pChannel[6]; long m_active; unsigned long m_numChannels; unsigned char m_storage[0xc10];
};
class CLimiter {
public:
    CLimiter() : m_writeIndex(0), m_state(0) { std::strcat(g_seq, "L"); ++g_limCtor; std::memset(m_delay, 0, sizeof m_delay); }
    virtual ~CLimiter() { }
    long m_delay[88]; unsigned long m_writeIndex; float m_peak; float m_envelope; long m_holdCount; long m_state; long m_releaseCount;
};
class CLimitedMixBuffer : public CMixBuffer { public: CLimitedMixBuffer() { } CLimiter m_limiters[6]; };
int main()
{
    CLimitedMixBuffer* b = new CLimitedMixBuffer;
    int ok = std::strcmp(g_seq, "MLLLLLL") == 0 && g_limCtor == 6 && sizeof(CMixBuffer) == 0xc34 && sizeof(CLimiter) == 0x17c
          && (const char*)&b->m_limiters[0] - (const char*)b == 0xc34 && b->m_limiters[5].m_state == 0;
    delete b;
    std::printf(ok ? "00c3efa0_TEST PASS\n" : "00c3efa0_TEST FAIL\n");
    return ok ? 0 : 1;
}

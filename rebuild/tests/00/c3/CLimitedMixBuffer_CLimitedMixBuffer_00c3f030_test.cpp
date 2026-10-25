// Behaviour test for 00c3f030: CLimitedMixBuffer::~CLimitedMixBuffer -- limiters destroyed last-to-first, then the base
#include <cstdio>
#include <cstring>
static char g_seq[16] = ""; static int g_next = 0;
class CMixBuffer { public: CMixBuffer() { } virtual ~CMixBuffer() { std::strcat(g_seq, "M"); } long* m_pChannel[6]; long m_active; unsigned long m_numChannels; unsigned char m_storage[0xc10]; };
class CLimiter {
public:
    CLimiter() : m_id(g_next++) { }
    virtual ~CLimiter() { char s[2] = { (char)('0' + m_id), 0 }; std::strcat(g_seq, s); }
    long m_id; long m_rest[0x17c / 4 - 2];
};
class CLimitedMixBuffer : public CMixBuffer { public: CLimitedMixBuffer() { } CLimiter m_limiters[6]; };
int main()
{
    CMixBuffer* b = new CLimitedMixBuffer;
    delete b;
    int ok = std::strcmp(g_seq, "543210M") == 0 && sizeof(CLimiter) == 0x17c;
    std::printf(ok ? "00c3f030_TEST PASS\n" : "00c3f030_TEST FAIL\n");
    return ok ? 0 : 1;
}

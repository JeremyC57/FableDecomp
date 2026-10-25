// Behaviour test for 00c57ef0: CMixBuffer::~CMixBuffer -- destroying a mix buffer leaves its accumulator state alone and runs the root destructor
#include <cstdio>
#include <new>
static int g_root = 0;
class CPKObject { public: virtual ~CPKObject() { ++g_root; } };
class CMixBuffer : public CPKObject {
public:
    CMixBuffer() : m_active(1), m_numChannels(6) { }
    virtual ~CMixBuffer() { }
    long* m_pChannel[6]; long m_active; unsigned long m_numChannels; unsigned char m_storage[0xc10];
};
int main()
{
    void* raw = operator new(sizeof(CMixBuffer));
    CMixBuffer* m = new (raw) CMixBuffer; m->~CMixBuffer();
    int ok = g_root == 1 && sizeof(CMixBuffer) == 0xc34 && ((CMixBuffer*)raw)->m_numChannels == 6;
    operator delete(raw);
    std::printf(ok ? "00c57ef0_TEST PASS\n" : "00c57ef0_TEST FAIL\n");
    return ok ? 0 : 1;
}

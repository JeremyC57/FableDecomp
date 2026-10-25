// Behaviour test for 00c59250: CMixBuffer scalar deleting destructor -- destroy, then free through the class allocator only when asked
#include <cstdio>
#include <cstdlib>
static int g_dtor = 0, g_free = 0;
class CPKObject {
public:
    virtual ~CPKObject() { }
    static void* operator new(unsigned int size) { return std::malloc(size); }
    static void operator delete(void* p) { ++g_free; std::free(p); }
};
class CMixBuffer : public CPKObject {
public:
    CMixBuffer() { }
    virtual ~CMixBuffer() { ++g_dtor; }
    long* m_pChannel[6]; long m_active; unsigned long m_numChannels; unsigned char m_storage[0xc10];
};
int main()
{
    CPKObject* p = new CMixBuffer;
    delete p;
    int ok = g_dtor == 1 && g_free == 1;
    std::printf(ok ? "00c59250_TEST PASS\n" : "00c59250_TEST FAIL\n");
    return ok ? 0 : 1;
}

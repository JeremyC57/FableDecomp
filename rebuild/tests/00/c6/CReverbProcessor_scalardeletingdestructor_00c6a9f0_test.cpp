// Behaviour test for 00c6a9f0: CReverbProcessor scalar deleting destructor -- destroy, then free
#include <cstdio>
static int g_dtor = 0, g_base = 0;
class CReverbProcessorBase { public: CReverbProcessorBase() { } virtual ~CReverbProcessorBase() { ++g_base; } };
class CReverbProcessor : public CReverbProcessorBase { public: CReverbProcessor() { } ~CReverbProcessor() { ++g_dtor; } unsigned char _body[0x294]; };
int main()
{
    CReverbProcessor* p = new CReverbProcessor;
    delete p;
    int ok = g_dtor == 1 && g_base == 1;
    std::printf(ok ? "00c6a9f0_TEST PASS\n" : "00c6a9f0_TEST FAIL\n");
    return ok ? 0 : 1;
}

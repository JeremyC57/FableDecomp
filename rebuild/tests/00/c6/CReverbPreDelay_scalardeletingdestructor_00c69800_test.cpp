// Behaviour test for 00c69800: CReverbPreDelay scalar deleting destructor -- destroy, then free
#include <cstdio>
static int g_dtor = 0, g_base = 0;
class CDspObject { public: CDspObject() { } virtual ~CDspObject() { ++g_base; } };
class CReverbPreDelay : public CDspObject { public: CReverbPreDelay() { } ~CReverbPreDelay() { ++g_dtor; } unsigned char _body[0x14]; };
int main()
{
    CReverbPreDelay* p = new CReverbPreDelay;
    delete p;
    int ok = g_dtor == 1 && g_base == 1;
    std::printf(ok ? "00c69800_TEST PASS\n" : "00c69800_TEST FAIL\n");
    return ok ? 0 : 1;
}

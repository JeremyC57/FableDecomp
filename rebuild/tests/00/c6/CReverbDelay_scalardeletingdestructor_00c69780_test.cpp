// Behaviour test for 00c69780: CReverbDelay scalar deleting destructor -- destroy, then free
#include <cstdio>
static int g_dtor = 0, g_base = 0;
class CDspObject { public: CDspObject() { } virtual ~CDspObject() { ++g_base; } };
class CReverbDelay : public CDspObject { public: CReverbDelay() { } ~CReverbDelay() { ++g_dtor; } unsigned char _body[0x10]; };
int main()
{
    CReverbDelay* p = new CReverbDelay;
    delete p;
    int ok = g_dtor == 1 && g_base == 1;
    std::printf(ok ? "00c69780_TEST PASS\n" : "00c69780_TEST FAIL\n");
    return ok ? 0 : 1;
}

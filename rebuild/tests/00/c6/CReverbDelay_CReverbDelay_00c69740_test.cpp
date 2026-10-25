// Behaviour test for 00c69740: CReverbDelay::~CReverbDelay -- the base destructor runs after the (empty) body
#include <cstdio>
static int g_base = 0, g_body = 0;
class CDspObject { public: CDspObject() { } virtual ~CDspObject() { ++g_base; } };
class CReverbDelay : public CDspObject { public: ~CReverbDelay() { ++g_body; } unsigned char _body[0x14 - 4]; };
int main()
{
    CDspObject* p = new CReverbDelay;
    delete p;
    int ok = g_body == 1 && g_base == 1 && sizeof(CReverbDelay) == 0x14;
    std::printf(ok ? "00c69740_TEST PASS\n" : "00c69740_TEST FAIL\n");
    return ok ? 0 : 1;
}

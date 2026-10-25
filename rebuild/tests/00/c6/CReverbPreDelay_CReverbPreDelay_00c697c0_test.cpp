// Behaviour test for 00c697c0: CReverbPreDelay::~CReverbPreDelay -- the base destructor runs after the (empty) body
#include <cstdio>
static int g_base = 0, g_body = 0;
class CDspObject { public: CDspObject() { } virtual ~CDspObject() { ++g_base; } };
class CReverbPreDelay : public CDspObject { public: ~CReverbPreDelay() { ++g_body; } unsigned char _body[0x18 - 4]; };
int main()
{
    CDspObject* p = new CReverbPreDelay;
    delete p;
    int ok = g_body == 1 && g_base == 1 && sizeof(CReverbPreDelay) == 0x18;
    std::printf(ok ? "00c697c0_TEST PASS\n" : "00c697c0_TEST FAIL\n");
    return ok ? 0 : 1;
}

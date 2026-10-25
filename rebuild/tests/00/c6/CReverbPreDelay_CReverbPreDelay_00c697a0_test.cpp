// Behaviour test for 00c697a0: CReverbPreDelay::CReverbPreDelay -- the DSP base runs once, the body is left alone
#include <cstdio>
#include <cstring>
#include <new>
static int g_base = 0;
class CDspObject { public: CDspObject() { ++g_base; } virtual ~CDspObject() { } };
class CReverbPreDelay : public CDspObject { public: CReverbPreDelay() { } unsigned char _body[0x18 - 4]; };
int main()
{
    unsigned char raw[sizeof(CReverbPreDelay)]; std::memset(raw, 0x3c, sizeof raw);
    CReverbPreDelay* p = new (raw) CReverbPreDelay;
    int ok = g_base == 1 && sizeof(CReverbPreDelay) == 0x18 && p->_body[3] == 0x3c;
    std::printf(ok ? "00c697a0_TEST PASS\n" : "00c697a0_TEST FAIL\n");
    return ok ? 0 : 1;
}

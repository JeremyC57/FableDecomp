// Behaviour test for 00c69720: CReverbDelay::CReverbDelay -- the DSP base runs once, the body is left alone
#include <cstdio>
#include <cstring>
#include <new>
static int g_base = 0;
class CDspObject { public: CDspObject() { ++g_base; } virtual ~CDspObject() { } };
class CReverbDelay : public CDspObject { public: CReverbDelay() { } unsigned char _body[0x14 - 4]; };
int main()
{
    unsigned char raw[sizeof(CReverbDelay)]; std::memset(raw, 0x3c, sizeof raw);
    CReverbDelay* p = new (raw) CReverbDelay;
    int ok = g_base == 1 && sizeof(CReverbDelay) == 0x14 && p->_body[3] == 0x3c;
    std::printf(ok ? "00c69720_TEST PASS\n" : "00c69720_TEST FAIL\n");
    return ok ? 0 : 1;
}

// Behaviour test for 00c69660: CDspGain::CDspGain -- the DSP base runs once, the gains are left alone
#include <cstdio>
#include <cstring>
#include <new>
static int g_base = 0;
class CDspObject { public: CDspObject() { ++g_base; } virtual ~CDspObject() { } };
class CDspGain : public CDspObject { public: CDspGain() { } float m_gain[4]; };
int main()
{
    unsigned char raw[sizeof(CDspGain)]; std::memset(raw, 0, sizeof raw);
    float* gains = (float*)(raw + 4); gains[2] = 0.5f;
    CDspGain* g = new (raw) CDspGain;
    int ok = g_base == 1 && g->m_gain[2] == 0.5f && sizeof(CDspGain) == 0x14;
    std::printf(ok ? "00c69660_TEST PASS\n" : "00c69660_TEST FAIL\n");
    return ok ? 0 : 1;
}

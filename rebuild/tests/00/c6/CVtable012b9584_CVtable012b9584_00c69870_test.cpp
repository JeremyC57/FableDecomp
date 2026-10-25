// Behaviour test for 00c69870: CVtable_012b9584::~CVtable_012b9584 -- only the base destructor runs; format defaults kept
#include <cstdio>
static int g_base = 0, g_dtor = 0;
class CSoundFormatBase {
public:
    CSoundFormatBase() { m_sampleRate = 48000; m_channels = 2; m_0c = 0; m_10 = 0; m_14 = 0; m_18 = 0; }
    virtual ~CSoundFormatBase() { ++g_base; }
    long m_channels, m_sampleRate, m_0c, m_10, m_14, m_18;
};
class CVtable_012b9584 : public CSoundFormatBase { public: CVtable_012b9584() { } ~CVtable_012b9584() { ++g_dtor; } };
int main()
{
    CVtable_012b9584* f = new CVtable_012b9584;
    int ok = f->m_channels == 2 && f->m_sampleRate == 48000 && sizeof(CVtable_012b9584) == 0x1c;
    CSoundFormatBase* b = f;
    delete b;
    ok = ok && g_dtor == 1 && g_base == 1;
    std::printf(ok ? "00c69870_TEST PASS\n" : "00c69870_TEST FAIL\n");
    return ok ? 0 : 1;
}

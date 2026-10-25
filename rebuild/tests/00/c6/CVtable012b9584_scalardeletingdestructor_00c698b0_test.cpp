// Behaviour test for 00c698b0: CVtable_012b9584 scalar deleting destructor -- destroy, then free
#include <cstdio>
static int g_dtor = 0, g_base = 0;
class CSoundFormatBase { public: CSoundFormatBase() { } virtual ~CSoundFormatBase() { ++g_base; } long m_channels, m_sampleRate, m_0c, m_10, m_14, m_18; };
class CVtable_012b9584 : public CSoundFormatBase { public: CVtable_012b9584() { } ~CVtable_012b9584() { ++g_dtor; } };
int main()
{
    CVtable_012b9584* p = new CVtable_012b9584;
    delete p;
    int ok = g_dtor == 1 && g_base == 1;
    std::printf(ok ? "00c698b0_TEST PASS\n" : "00c698b0_TEST FAIL\n");
    return ok ? 0 : 1;
}

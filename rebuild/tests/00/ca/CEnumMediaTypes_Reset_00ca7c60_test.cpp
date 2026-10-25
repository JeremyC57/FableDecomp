// Behaviour test for 00ca7c60: CEnumMediaTypes::Reset -- position 0, version re-read from the pin, NOERROR
#include <cstdio>
static int g_calls = 0;
class CBasePin { public: long m_TypeVersion; virtual long GetMediaTypeVersion() { ++g_calls; return m_TypeVersion; } };
class CEnumMediaTypes {
public:
    int m_Position; CBasePin* m_pPin; long m_Version; long m_cRef;
    long Reset() { m_Position = 0; m_Version = m_pPin->GetMediaTypeVersion(); return 0; }
};
int main()
{
    CBasePin pin; pin.m_TypeVersion = 41; CEnumMediaTypes e; e.m_Position = 3; e.m_pPin = &pin; e.m_Version = 2; e.m_cRef = 1;
    int ok = e.Reset() == 0 && e.m_Position == 0 && e.m_Version == 41 && g_calls == 1 && e.m_cRef == 1;
    std::printf(ok ? "00ca7c60_TEST PASS\n" : "00ca7c60_TEST FAIL\n");
    return ok ? 0 : 1;
}

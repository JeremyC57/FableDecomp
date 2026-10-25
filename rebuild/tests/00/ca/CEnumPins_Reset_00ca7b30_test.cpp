// Behaviour test for 00ca7b30: CEnumPins::Reset -- version and count re-read, position rewound, cache emptied
#include <cstdio>
static int g_removeAll = 0;
class CBaseFilter { public: long m_version; int m_count; virtual long GetPinVersion() { return m_version; } virtual int GetPinCount() { return m_count; } };
struct CPinList { int m_items; void RemoveAll() { ++g_removeAll; m_items = 0; } };
class CEnumPins {
public:
    int m_Position; int m_PinCount; CBaseFilter* m_pFilter; long m_Version; long m_cRef; CPinList m_PinCache;
    long __stdcall Reset() { m_Version = m_pFilter->GetPinVersion(); m_PinCount = m_pFilter->GetPinCount(); m_Position = 0; m_PinCache.RemoveAll(); return 0; }
};
int main()
{
    CBaseFilter f; f.m_version = 7; f.m_count = 3;
    CEnumPins e; e.m_Position = 2; e.m_PinCount = 1; e.m_pFilter = &f; e.m_Version = 1; e.m_cRef = 1; e.m_PinCache.m_items = 2;
    long hr = e.Reset();
    int ok = hr == 0 && e.m_Version == 7 && e.m_PinCount == 3 && e.m_Position == 0 && g_removeAll == 1 && e.m_PinCache.m_items == (1 ? 0 : 2);
    std::printf(ok ? "00ca7b30_TEST PASS\n" : "00ca7b30_TEST FAIL\n");
    return ok ? 0 : 1;
}

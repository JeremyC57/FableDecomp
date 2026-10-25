// Behaviour test for 00ca8660: CBaseOutputPin::BreakConnect -- decommit+release the allocator (failure returned), release the input pin
#include <cstdio>
static int g_relA = 0, g_relI = 0, g_decommit = 0; static long g_decommitHr = 0;
struct IMemAllocator { virtual long Decommit() { ++g_decommit; return g_decommitHr; } virtual unsigned long Release() { return ++g_relA; } };
struct IMemInputPin { virtual unsigned long Release() { return ++g_relI; } };
class CBaseOutputPin {
public:
    IMemAllocator* m_pAllocator; IMemInputPin* m_pInputPin;
    long BreakConnect()
    {
        if (m_pAllocator) {
            long hr = m_pAllocator->Decommit();
            if (hr < 0) return hr;
            m_pAllocator->Release();
            m_pAllocator = 0;
        }
        if (m_pInputPin) { m_pInputPin->Release(); m_pInputPin = 0; }
        return 0;
    }
};
int main()
{
    IMemAllocator a; IMemInputPin i; CBaseOutputPin p; p.m_pAllocator = &a; p.m_pInputPin = &i; int ok = 1;
    g_decommitHr = (long)0x80040211L; ok &= p.BreakConnect() == (long)0x80040211L && p.m_pAllocator == &a && p.m_pInputPin == &i && g_relA == 0 && g_relI == 0;
    g_decommitHr = 0; ok &= p.BreakConnect() == 0 && p.m_pAllocator == 0 && p.m_pInputPin == 0 && g_relA == 1 && g_relI == 1;
    ok &= p.BreakConnect() == 0 && g_decommit == 2 && g_relA == 1 && g_relI == 1;
    std::printf(ok ? "00ca8660_TEST PASS\n" : "00ca8660_TEST FAIL\n");
    return ok ? 0 : 1;
}

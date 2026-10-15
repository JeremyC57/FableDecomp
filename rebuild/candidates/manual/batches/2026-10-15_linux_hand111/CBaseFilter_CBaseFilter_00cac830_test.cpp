#include <cstdio>
// Behaviour test for CBaseFilter::CBaseFilter (0x00CAC830): DirectShow base filter initial state.
struct GUID { unsigned long Data1; unsigned short Data2, Data3; unsigned char Data4[8]; };
struct CCritSec { int dummy; };
struct IUnknown { int dummy; };
struct CUnknown {
    IUnknown* m_pUnknown; long m_cRef;
    CUnknown(const char*, IUnknown* pUnk) : m_pUnknown(pUnk ? pUnk : (IUnknown*)this), m_cRef(0) { }
};
struct CRefTime { long long m_time; CRefTime() { m_time = 0; } };
struct CBaseFilter : CUnknown {
    int m_State; void* m_pClock; CRefTime m_tStart; GUID m_clsid; CCritSec* m_pLock; unsigned short* m_pName; void* m_pGraph; void* m_pSink; long m_PinVersion;
    CBaseFilter(const char* pName, IUnknown* pUnk, CCritSec* pLock, const GUID& clsid)
        : CUnknown(pName, pUnk), m_State(0), m_pClock(0), m_tStart(), m_clsid(clsid), m_pLock(pLock), m_pName(0), m_pGraph(0), m_pSink(0), m_PinVersion(1) { }
};
int main()
{
    GUID g; g.Data1 = 0x12345678; g.Data2 = 1; g.Data3 = 2; for (int i = 0; i < 8; ++i) g.Data4[i] = (unsigned char)(i * 3);
    CCritSec lock; IUnknown outer;
    CBaseFilter f("filter", &outer, &lock, g);
    int fails = 0;
    if (f.m_pUnknown != &outer || f.m_cRef != 0 || f.m_State != 0 || f.m_pClock || f.m_tStart.m_time != 0) ++fails;
    if (f.m_clsid.Data1 != 0x12345678 || f.m_clsid.Data3 != 2 || f.m_clsid.Data4[7] != 21) ++fails;
    if (f.m_pLock != &lock || f.m_pName || f.m_pGraph || f.m_pSink || f.m_PinVersion != 1) ++fails;
    CBaseFilter self("filter", 0, &lock, g);
    if (self.m_pUnknown != (IUnknown*)(CUnknown*)&self) ++fails;
    if (fails) { std::printf("00cac830_TEST FAIL\n"); return 1; }
    std::printf("00cac830_TEST PASS\n");
    return 0;
}

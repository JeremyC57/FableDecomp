// Behaviour test for 00cafce0 CRendererPosPassThru constructor (labelled
// CCompressedMemoryDataInputStream): the pin is kept, a null pin reports E_POINTER through *phr, the
// CUnknown outer pointer defaults to the object itself, and the renderer's media times start at zero
// with the reset flag set.
#include <cstdio>
#ifdef _MSC_VER
typedef __int64 REFTIME64;
#else
typedef long long REFTIME64;
#endif

static long g_objects = 0;
struct CUnknown {
    void* m_pUnknown; long m_cRef;
    CUnknown(const char*, void* outer) { ++g_objects; m_pUnknown = outer ? outer : (void*)this; m_cRef = 0; }
    virtual ~CUnknown() { }
};
struct CBaseDispatch { void* m_pti; CBaseDispatch() : m_pti(0) { } };
struct CMediaPosition : CUnknown {
    CBaseDispatch m_basedisp;
    CMediaPosition(const char* n, void* o) : CUnknown(n, o) { }
};
struct CPosPassThru : CMediaPosition {
    void* m_pPin;
    CPosPassThru(const char* n, void* o, long* phr, void* pin) : CMediaPosition(n, o), m_pPin(pin)
    {
        if (pin == 0) { *phr = (long)0x80004003L; return; }
    }
};
struct CCritSec { int m_initialised; CCritSec() : m_initialised(1) { } };
struct CRendererPosPassThru : CPosPassThru {
    CCritSec m_PositionLock; REFTIME64 m_StartMedia; REFTIME64 m_EndMedia; long m_bReset;
    CRendererPosPassThru(const char* n, void* o, long* phr, void* pin)
        : CPosPassThru(n, o, phr, pin), m_StartMedia(0), m_EndMedia(0), m_bReset(1) { }
};

int main()
{
    long hr = 0; int pin = 0, outer = 0;
    CRendererPosPassThru a("a", 0, &hr, &pin);
    bool ok = hr == 0 && a.m_pPin == &pin && a.m_pUnknown == (void*)static_cast<CUnknown*>(&a) && a.m_cRef == 0
        && a.m_basedisp.m_pti == 0 && a.m_StartMedia == 0 && a.m_EndMedia == 0 && a.m_bReset == 1
        && a.m_PositionLock.m_initialised == 1;
    CRendererPosPassThru b("b", &outer, &hr, 0);
    ok = ok && hr == (long)0x80004003L && b.m_pUnknown == &outer && b.m_bReset == 1 && g_objects == 2;
    if (!ok) { std::printf("00cafce0_TEST FAIL\n"); return 1; }
    std::printf("00cafce0_TEST PASS\n");
    return 0;
}

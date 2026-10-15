// CBaseFilter::StreamTime(CRefTime& rtStream) -- DirectShow BaseClasses: VFW_E_NO_CLOCK without a reference clock; else the clock's current time minus the stream start m_tStart
// Leaf: ?StreamTime@CBaseFilter
// Retail: Fable.exe @ 0x00CA77A0, 65 bytes (+ int3 padding). Primary vtable slot 4 of CTextureRenderer and two more filters.
// Label: the manifest's "CTextureRenderer::vfunc_4".
// Callee: IReferenceClock::GetTime (COM slot 3, `call [ecx+0Ch]`, `this` pushed).
// Identification: Fable links Microsoft's DirectShow BaseClasses (amfilter / renbase / combase) for its movie renderer
// CTextureRenderer (the DX9 SDK Texture3D9 sample class, primary vtable 0x0129D08C); the primary-vtable slot of this body
// and its callers' slots line up with the BaseClasses declaration order (CUnknown 0-3, CBaseFilter StreamTime 4,
// CBaseRenderer SourceThreadCanWait 10, OnRenderStart/OnRenderEnd 14/15, ScheduleSample 21, GetSampleTimes 22,
// ShouldDrawSampleNow 23, Render 42, DoRenderSample 43, CheckMediaType 44) and with the member offsets (CBaseFilter:
// m_pClock +0x18, m_tStart +0x20, m_clsid +0x28; CBaseRenderer: m_ThreadSignal +0x58, m_bStreaming +0x64).
// Shape: amfilter.cpp verbatim; m_pClock is re-read after the null test (the test's join), `rtStream -= m_tStart` is the
// sub/sbb pair through the CRefTime's REFERENCE_TIME.
typedef __int64 REFERENCE_TIME;

struct IReferenceClock {
    virtual long __stdcall QueryInterface(const void* iid, void** object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual long __stdcall GetTime(REFERENCE_TIME* pTime);   // slot 3 (+0x0c)
};

class CRefTime {
public:
    REFERENCE_TIME m_time;
    CRefTime& operator-=(const CRefTime& rt) { m_time -= rt.m_time; return *this; }
};

class CBaseFilter {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual long StreamTime(CRefTime& rtStream);   // slot 4
    unsigned char    _pad_0x08[0x10];              // +0x08 rest of CUnknown, IBaseFilter, IAMovieSetup, m_State (MSVC pads the
                                                   // vfptr this local model introduces to the class's 8-byte alignment)
    IReferenceClock* m_pClock;                     // +0x18
    CRefTime         m_tStart;                     // +0x20
};

#define S_OK           0L
#define VFW_E_NO_CLOCK ((long)0x80040213L)
#define FAILED(hr)     ((long)(hr) < 0)

long CBaseFilter::StreamTime(CRefTime& rtStream)
{
    // Caller must lock for synchronization
    if (m_pClock == 0)
        return VFW_E_NO_CLOCK;

    // get the current reference time
    long hr = m_pClock->GetTime((REFERENCE_TIME*)&rtStream);
    if (FAILED(hr))
        return hr;

    // subtract the stream offset to get stream time
    rtStream -= m_tStart;
    return S_OK;
}

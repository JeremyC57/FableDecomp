// CBaseRenderer::GetSampleTimes(IMediaSample* pMediaSample, REFERENCE_TIME* pStartTime, REFERENCE_TIME* pEndTime) -- DirectShow BaseClasses: reads the sample's time stamps; S_OK when it has none or there is no clock, VFW_E_START_TIME_AFTER_END when end < start, else ShouldDrawSampleNow
// Leaf: ?GetSampleTimes@CBaseRenderer
// Retail: Fable.exe @ 0x00CA49F0, 92 bytes (+ int3 padding). In three vtables (CTextureRenderer primary slot 22 and two
// more renderer vtables 0x012BF4F0 / 0x012BF630 area). Label: the manifest's "CTextureRenderer::vfunc_22".
// Callees: IMediaSample::GetTime (COM slot 5, `call [eax+14h]`, `this` pushed), ShouldDrawSampleNow (slot 23, +0x5c).
// Identification: Fable links Microsoft's DirectShow BaseClasses (amfilter / renbase / combase) for its movie renderer
// CTextureRenderer (the DX9 SDK Texture3D9 sample class, primary vtable 0x0129D08C); the primary-vtable slot of this body
// and its callers' slots line up with the BaseClasses declaration order (CUnknown 0-3, CBaseFilter StreamTime 4,
// CBaseRenderer SourceThreadCanWait 10, OnRenderStart/OnRenderEnd 14/15, ScheduleSample 21, GetSampleTimes 22,
// ShouldDrawSampleNow 23, Render 42, DoRenderSample 43, CheckMediaType 44) and with the member offsets (CBaseFilter:
// m_pClock +0x18, m_tStart +0x20, m_clsid +0x28; CBaseRenderer: m_ThreadSignal +0x58, m_bStreaming +0x64).
// Shape: renbase.cpp verbatim (minus the debug ASSERTs): `if (SUCCEEDED(GetTime)) { if (*pEnd < *pStart) return
// VFW_E_START_TIME_AFTER_END; } else return S_OK;` then the clock test (the 64-bit compare is the hi-word jg/jl +
// unsigned lo-word jae ladder).
typedef __int64 REFERENCE_TIME;

struct IMediaSample {
    virtual long __stdcall QueryInterface(const void* iid, void** object);   // slot 0
    virtual unsigned long __stdcall AddRef();                                // slot 1
    virtual unsigned long __stdcall Release();                               // slot 2
    virtual long __stdcall GetPointer(unsigned char** buffer);               // slot 3
    virtual long __stdcall GetSize();                                        // slot 4
    virtual long __stdcall GetTime(REFERENCE_TIME* start, REFERENCE_TIME* end);   // slot 5 (+0x14)
};
struct IReferenceClock;

class CBaseRenderer {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual long GetSampleTimes(IMediaSample* pMediaSample, REFERENCE_TIME* pStartTime, REFERENCE_TIME* pEndTime);   // slot 22
    virtual long ShouldDrawSampleNow(IMediaSample* pMediaSample, REFERENCE_TIME* ptrStart, REFERENCE_TIME* ptrEnd);  // slot 23
    unsigned char    _pad_0x04[0x14];              // +0x04 CUnknown / IBaseFilter / IAMovieSetup / m_State
    IReferenceClock* m_pClock;                     // +0x18 (CBaseFilter)
};

#define S_OK                       0L
#define VFW_E_START_TIME_AFTER_END ((long)0x80040228L)
#define SUCCEEDED(hr)              ((long)(hr) >= 0)

long CBaseRenderer::GetSampleTimes(IMediaSample* pMediaSample, REFERENCE_TIME* pStartTime, REFERENCE_TIME* pEndTime)
{
    // Check the start time is greater than the end time
    if (SUCCEEDED(pMediaSample->GetTime(pStartTime, pEndTime))) {
        if (*pEndTime < *pStartTime)
            return VFW_E_START_TIME_AFTER_END;
    } else {
        // no time set in the sample... draw it now?
        return S_OK;
    }

    // Can't synchronise without a clock so we return S_OK which tells the caller that the sample should be rendered
    // immediately without going through the overhead of setting a timer advise link with the clock
    if (m_pClock == 0)
        return S_OK;
    return ShouldDrawSampleNow(pMediaSample, pStartTime, pEndTime);
}
// CBaseRenderer::Render(IMediaSample* pMediaSample) -- DirectShow BaseClasses: S_FALSE without a sample or when not streaming, else OnRenderStart / DoRenderSample / OnRenderEnd and NOERROR
// Leaf: ?Render@CBaseRenderer
// Retail: Fable.exe @ 0x00CA4B20, 63 bytes (+ int3 padding). In three vtables (CTextureRenderer primary slot 42).
// Label: the manifest's "CTextureRenderer::vfunc_42".
// Callees: OnRenderStart (primary slot 14, +0x38), DoRenderSample (slot 43, +0xac), OnRenderEnd (slot 15, +0x3c).
// Identification: Fable links Microsoft's DirectShow BaseClasses (amfilter / renbase / combase) for its movie renderer
// CTextureRenderer (the DX9 SDK Texture3D9 sample class, primary vtable 0x0129D08C); the primary-vtable slot of this body
// and its callers' slots line up with the BaseClasses declaration order (CUnknown 0-3, CBaseFilter StreamTime 4,
// CBaseRenderer SourceThreadCanWait 10, OnRenderStart/OnRenderEnd 14/15, ScheduleSample 21, GetSampleTimes 22,
// ShouldDrawSampleNow 23, Render 42, DoRenderSample 43, CheckMediaType 44) and with the member offsets (CBaseFilter:
// m_pClock +0x18, m_tStart +0x20, m_clsid +0x28; CBaseRenderer: m_ThreadSignal +0x58, m_bStreaming +0x64).
// Shape: renbase.cpp verbatim (the two S_FALSE exits share one tail); the vtable is re-read for every virtual call.
struct IMediaSample;

class CBaseRenderer {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13();
    virtual void OnRenderStart(IMediaSample* pMediaSample);   // slot 14 (+0x38)
    virtual void OnRenderEnd(IMediaSample* pMediaSample);     // slot 15 (+0x3c)
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33();
    virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42();
    virtual long DoRenderSample(IMediaSample* pMediaSample);  // slot 43 (+0xac)
    long Render(IMediaSample* pMediaSample);
    unsigned char _pad_0x04[0x60];                 // +0x04 .. CBaseFilter, m_pPosition, the three CAMEvents, m_bAbort
    int           m_bStreaming;                    // +0x64 BOOL
};

#define NOERROR 0L
#define S_FALSE 1L

long CBaseRenderer::Render(IMediaSample* pMediaSample)
{
    // If the media sample is NULL then we will have been notified by the clock that another sample is ready but in
    // the mean time someone has stopped us streaming which causes the next sample to be released
    if (pMediaSample == 0)
        return S_FALSE;

    // If we have stopped streaming then don't render any more samples
    if (m_bStreaming == 0)
        return S_FALSE;

    // Time how long the rendering takes
    OnRenderStart(pMediaSample);
    DoRenderSample(pMediaSample);
    OnRenderEnd(pMediaSample);
    return NOERROR;
}

// CBaseVideoRenderer::ScheduleSample(IMediaSample* pMediaSample) -- DirectShow BaseClasses: the base CBaseRenderer::ScheduleSample decides; a refused sample counts as a dropped frame (m_cFramesDropped)
// Leaf: ?ScheduleSample@CBaseVideoRenderer
// Retail: Fable.exe @ 0x00CA5CF0, 38 bytes. In two vtables (CTextureRenderer primary slot 21).
// Label: the manifest's "CTextureRenderer::vfunc_21".
// Callees: CBaseRenderer::ScheduleSample @ 0x00CA4AA0 (non-virtual qualified call; autonamed "Audio_ProcessEventWithCallback").
// Identification: Fable links Microsoft's DirectShow BaseClasses (amfilter / renbase / combase) for its movie renderer
// CTextureRenderer (the DX9 SDK Texture3D9 sample class, primary vtable 0x0129D08C); the primary-vtable slot of this body
// and its callers' slots line up with the BaseClasses declaration order (CUnknown 0-3, CBaseFilter StreamTime 4,
// CBaseRenderer SourceThreadCanWait 10, OnRenderStart/OnRenderEnd 14/15, ScheduleSample 21, GetSampleTimes 22,
// ShouldDrawSampleNow 23, Render 42, DoRenderSample 43, CheckMediaType 44) and with the member offsets (CBaseFilter:
// m_pClock +0x18, m_tStart +0x20, m_clsid +0x28; CBaseRenderer: m_ThreadSignal +0x58, m_bStreaming +0x64).
// Retail CBaseVideoRenderer keeps m_cFramesDropped at +0x120.
// Shape: renbase.cpp verbatim: `if (bDrawImage == FALSE) { ++m_cFramesDropped; return FALSE; } return TRUE;`.
struct IMediaSample;

class CBaseRenderer {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20();
    virtual int ScheduleSample(IMediaSample* pMediaSample);   // slot 21; CBaseRenderer's 0x00CA4AA0
};

class CBaseVideoRenderer : public CBaseRenderer {
public:
    unsigned char _pad_0x04[0x11c];                // +0x04 CBaseRenderer, IQualProp (+0xe0), IQualityControl, statistics
    int           m_cFramesDropped;                // +0x120
    virtual int ScheduleSample(IMediaSample* pMediaSample);
};

int CBaseVideoRenderer::ScheduleSample(IMediaSample* pMediaSample)
{
    // We override ShouldDrawSampleNow to add quality management
    int bDrawImage = CBaseRenderer::ScheduleSample(pMediaSample);
    if (bDrawImage == 0) {
        ++m_cFramesDropped;
        return 0;
    }

    // m_cFramesDrawn must NOT be updated here.  It has to be updated in RecordFrameLateness at the same time as the
    // other statistics.
    return 1;
}

// CBaseRenderer::SourceThreadCanWait(BOOL bCanWait) -- DirectShow BaseClasses: lets the source thread block (reset m_ThreadSignal) or releases it (set m_ThreadSignal); NOERROR
// Leaf: ?SourceThreadCanWait@CBaseRenderer
// Retail: Fable.exe @ 0x00CA6580, 37 bytes (+ int3 padding). In three vtables (CTextureRenderer primary slot 10).
// Label: the manifest's "CTextureRenderer::vfunc_10".
// Callees: ResetEvent (IAT 0x0143FED4) / SetEvent (IAT 0x0143FDE0) through the inline CAMEvent::Reset / Set on m_ThreadSignal.
// Identification: Fable links Microsoft's DirectShow BaseClasses (amfilter / renbase / combase) for its movie renderer
// CTextureRenderer (the DX9 SDK Texture3D9 sample class, primary vtable 0x0129D08C); the primary-vtable slot of this body
// and its callers' slots line up with the BaseClasses declaration order (CUnknown 0-3, CBaseFilter StreamTime 4,
// CBaseRenderer SourceThreadCanWait 10, OnRenderStart/OnRenderEnd 14/15, ScheduleSample 21, GetSampleTimes 22,
// ShouldDrawSampleNow 23, Render 42, DoRenderSample 43, CheckMediaType 44) and with the member offsets (CBaseFilter:
// m_pClock +0x18, m_tStart +0x20, m_clsid +0x28; CBaseRenderer: m_ThreadSignal +0x58, m_bStreaming +0x64).
// Shape: renbase.cpp verbatim (`if (bCanWait == TRUE)`), each arm with its own `return NOERROR` tail after the
// `__stdcall` import call on the event handle (CAMEvent holds only the HANDLE).
extern "C" __declspec(dllimport) int __stdcall SetEvent(void* event);     // IAT 0x0143FDE0
extern "C" __declspec(dllimport) int __stdcall ResetEvent(void* event);   // IAT 0x0143FED4

class CAMEvent {
public:
    void* m_hEvent;                                // HANDLE
    void Set() { SetEvent(m_hEvent); }
    void Reset() { ResetEvent(m_hEvent); }
};

class CBaseRenderer {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual long SourceThreadCanWait(int bCanWait);   // slot 10
    unsigned char _pad_0x04[0x54];                 // +0x04 CBaseFilter, m_pPosition, m_RenderEvent
    CAMEvent      m_ThreadSignal;                  // +0x58
};

#define NOERROR 0L

long CBaseRenderer::SourceThreadCanWait(int bCanWait)
{
    if (bCanWait == 1) {
        m_ThreadSignal.Reset();
    } else {
        m_ThreadSignal.Set();
    }
    return NOERROR;
}
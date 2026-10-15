// CBaseVideoRenderer::get_DevSyncOffset(int* piResult) -- DirectShow BaseClasses IQualProp::get_DevSyncOffset: the standard deviation in ms of the per-frame sync offset since streaming started (GetStdDev over m_cFramesDrawn - 1 samples of m_iSumSqAcc / m_iTotAcc)
// Leaf: ?get_DevSyncOffset@CBaseVideoRenderer
// Retail: Fable.exe @ 0x00CA6020, 44 bytes (+ int3 padding). IQualProp vtable 0x0129D008 slot 8 (and 0x012BF5xx).
// Label: the manifest's "CTextureRenderer::vfunc_8".
// Callee: CBaseVideoRenderer::GetStdDev(int nSamples, int* piResult, LONGLONG llSumSq, LONGLONG iTot) @ 0x00CA5F50
// (autonamed "AudioDevice_ComputeLatencyDelta").
// Identification: Fable links Microsoft's DirectShow BaseClasses (amfilter / renbase / combase) for its movie renderer
// CTextureRenderer (the DX9 SDK Texture3D9 sample class, primary vtable 0x0129D08C); the primary-vtable slot of this body
// and its callers' slots line up with the BaseClasses declaration order (CUnknown 0-3, CBaseFilter StreamTime 4,
// CBaseRenderer SourceThreadCanWait 10, OnRenderStart/OnRenderEnd 14/15, ScheduleSample 21, GetSampleTimes 22,
// ShouldDrawSampleNow 23, Render 42, DoRenderSample 43, CheckMediaType 44) and with the member offsets (CBaseFilter:
// m_pClock +0x18, m_tStart +0x20, m_clsid +0x28; CBaseRenderer: m_ThreadSignal +0x58, m_bStreaming +0x64).
// The IQualProp base sits at +0xe0 of CBaseVideoRenderer: the STDMETHODIMP override is `__stdcall` with `this` = that
// base sub-object on the stack, so the members read are at sub-object offsets (m_cFramesDrawn +0x44, m_iTotAcc +0x48, m_iSumSqAcc +0x50) and GetStdDev
// is called on `this - 0xe0` (`lea ecx,[eax-0E0h]`).
// Shape: renbase.cpp verbatim; the two 64-bit sums are pushed as dword pairs (high word first) interleaved with the loads.
typedef __int64 LONGLONG;

class CBaseRenderer {                             // retail 0xe0 bytes
public:
    virtual void v00();
    unsigned char _body[0xdc];
};

struct IQualProp {                                // at +0xe0
    virtual long __stdcall QueryInterface(const void* iid, void** object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual long __stdcall get_FramesDroppedInRenderer(int* pcFrames);   // slot 3
    virtual long __stdcall get_FramesDrawn(int* pcFramesDrawn);          // slot 4
    virtual long __stdcall get_AvgFrameRate(int* piAvgFrameRate);        // slot 5
    virtual long __stdcall get_Jitter(int* piJitter);                    // slot 6
    virtual long __stdcall get_AvgSyncOffset(int* piAvg);                // slot 7
    virtual long __stdcall get_DevSyncOffset(int* piDev);                // slot 8
};

class CBaseVideoRenderer : public CBaseRenderer, public IQualProp {
public:
    unsigned char _pad_0xe4[0x124 - 0xe4];
    int      m_cFramesDrawn;                       // +0x124 (sub-object +0x44)
    LONGLONG m_iTotAcc;                            // +0x128 (+0x48)
    LONGLONG m_iSumSqAcc;                          // +0x130 (+0x50)
    unsigned char _pad_0x138[0x140 - 0x138];
    LONGLONG m_iSumSqFrameTime;                    // +0x140 (+0x60)
    LONGLONG m_iSumFrameTime;                      // +0x148 (+0x68)

    long GetStdDev(int nSamples, int* piResult, LONGLONG llSumSq, LONGLONG iTot);   // 0x00CA5F50
    long __stdcall get_DevSyncOffset(int* piResult);
};

long __stdcall CBaseVideoRenderer::get_DevSyncOffset(int* piResult)
{
    return GetStdDev(m_cFramesDrawn - 1, piResult, m_iSumSqAcc, m_iTotAcc);
}
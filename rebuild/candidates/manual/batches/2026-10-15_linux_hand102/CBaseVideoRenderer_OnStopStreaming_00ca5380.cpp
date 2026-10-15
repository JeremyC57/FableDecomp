// CBaseVideoRenderer::OnStopStreaming() -- DirectShow base-class hook: turns the streaming start time into the elapsed streaming time, returns NOERROR
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA5380, length 25 bytes. Vtable 0x0129D08C slot 17 (the CTextureRenderer
// video renderer, derived from the DirectShow SDK's CBaseVideoRenderer; also referenced from 0x012BF61C).
// Callee: winmm timeGetTime through its import slot @ 0x014404B4 (`call [__imp__timeGetTime@0]`).
// Identity: the DirectShow base classes (renbase.cpp) define exactly this body:
//   HRESULT CBaseVideoRenderer::OnStopStreaming() { m_tStreamingStart = timeGetTime() - m_tStreamingStart; return NOERROR; }
// The catalog label "CTCTalk::IsTalking" is propagated over an unrelated body and rejected. m_tStreamingStart (int) sits at
// +0x158 of the retail renderer; the rest of the object is opaque here.
// Shape: the import must be `extern "C" __declspec(dllimport)` (no SDK header) for the indirect call; `xor eax,eax` is the
// long HRESULT result.
extern "C" __declspec(dllimport) unsigned long __stdcall timeGetTime(void);   // 0x014404B4 import slot

typedef long HRESULT;

struct CBaseVideoRenderer {
    virtual void v00();
    unsigned char _pad_0x04[0x154];          // CBaseRenderer state, quality-management counters
    int           m_tStreamingStart;         // +0x158
    HRESULT OnStopStreaming();
};

HRESULT CBaseVideoRenderer::OnStopStreaming()
{
    m_tStreamingStart = timeGetTime() - m_tStreamingStart;
    return 0;   // NOERROR
}

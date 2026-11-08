// CBaseRenderer::CancelNotification() -- cancel any clock advise (Unadvise + SignalTimerFired), reset the render event; S_OK when an advise was live, else S_FALSE (DirectShow BaseClasses)
// Vendor-proof: strmbase-dxsdk-sum2004(BaseClasses/renbase.cpp) ?CancelNotification@CBaseRenderer@@UAEJXZ -- 48 bytes verified against Fable.exe 0x00CA4A70
// Leaf: ?CancelNotification@CBaseRenderer@@UAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA4A70, 48 bytes, thiscall (__fastcall this=ecx).
// Identity (vendor naming wave V1, proven): BaseClasses renbase.h declaration order: CBaseRenderer::CancelNotification is primary-vtable slot 27; retail CBaseRenderer vtable 0x012BF498 slot 27 (its ??_G 0x00CA7340) and CTextureRenderer vtable 0x0129D08C (RTTI) slot 27 hold 0x00CA4A70.
//   Body: the BaseClasses source of that member (DX9-era SDK; the Win7 sample text differs only where noted), modelled on the
//   shared engine/CBaseVideoRenderer.h class (renbase.h layout) instead of a local stand-in.
//   Was: CAsyncRequester_00ca4a70::CancelRequest() -- cancel the pending request at +0x68 through the device interface at +0x18 (`__stdcall` slot 6, COM style), reset the completion event at +0x54, report whet (Leaf none, module CAsyncRequester_00ca4a70).
#define FABLE_RETAIL_CBASEVIDEORENDERER
#define FABLE_RETAIL_CBASEVIDEORENDERER_OVERRIDES
#include "engine/CBaseVideoRenderer.h"
long CBaseRenderer::CancelNotification()
{
    unsigned long dwAdvise = m_dwAdvise;
    // Have we a live advise link
    if (m_dwAdvise) {
        m_pClock->Unadvise(m_dwAdvise);
        SignalTimerFired();
    }
    // Clear the event and return our status
    m_RenderEvent.Reset();
    return (dwAdvise ? S_OK : S_FALSE);
}

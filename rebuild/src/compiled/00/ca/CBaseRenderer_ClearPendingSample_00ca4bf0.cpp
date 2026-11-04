// CBaseRenderer::ClearPendingSample() -- under the renderer lock, release and clear the held media sample (DirectShow BaseClasses)
// Leaf: ?ClearPendingSample@CBaseRenderer@@UAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA4BF0, 97 bytes, thiscall (__fastcall this=ecx).
// Identity (vendor naming wave V1, proven): BaseClasses renbase.h declaration order: CBaseRenderer::ClearPendingSample is primary-vtable slot 28; retail CBaseRenderer vtable 0x012BF498 slot 28 (its ??_G 0x00CA7340) and CTextureRenderer vtable 0x0129D08C (RTTI) slot 28 hold 0x00CA4BF0.
//   Body: the BaseClasses source of that member (DX9-era SDK; the Win7 sample text differs only where noted), modelled on the
//   shared engine/CBaseVideoRenderer.h class (renbase.h layout) instead of a local stand-in.
//   Was: CGuiWindowManager_00ca4bf0::ReleaseForegroundWindow() -- EH-framed: holds the critical section at +0x94, releases the COM window at +0x6c and clears it when present, returns 0 (Leaf ReleaseForegroundWindow_00ca4bf0, module CTextureRenderer).
#define FABLE_RETAIL_CBASEVIDEORENDERER
#define FABLE_RETAIL_CBASEVIDEORENDERER_OVERRIDES
#include "engine/CBaseVideoRenderer.h"
long CBaseRenderer::ClearPendingSample()
{
    CAutoLock cRendererLock(&m_RendererLock);
    if (m_pMediaSample) {
        m_pMediaSample->Release();
        m_pMediaSample = 0;
    }
    return NOERROR;
}

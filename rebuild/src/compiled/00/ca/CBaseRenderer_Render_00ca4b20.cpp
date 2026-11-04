// CBaseRenderer::Render(struct IMediaSample*) -- render one sample: S_FALSE for no sample or when not streaming, else OnRenderStart / DoRenderSample / OnRenderEnd (DirectShow BaseClasses)
// Leaf: ?Render@CBaseRenderer@@UAEJPAUIMediaSample@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA4B20, 63 bytes, thiscall (__fastcall this=ecx).
// Identity (vendor naming wave V1, proven): BaseClasses renbase.h declaration order: CBaseRenderer::Render is primary-vtable slot 42; retail CBaseRenderer vtable 0x012BF498 slot 42 (its ??_G 0x00CA7340) and CTextureRenderer vtable 0x0129D08C (RTTI) slot 42 hold 0x00CA4B20.
//   Body: the BaseClasses source of that member (DX9-era SDK; the Win7 sample text differs only where noted), modelled on the
//   shared engine/CBaseVideoRenderer.h class (renbase.h layout) instead of a local stand-in.
//   Was: CBaseRenderer::Render(IMediaSample* pMediaSample) -- DirectShow BaseClasses: S_FALSE without a sample or when not streaming, else OnRenderStart / DoRenderSample / OnRenderEnd and NOERROR (Leaf ?Render@CBaseRenderer, module CBaseRenderer).
#define FABLE_RETAIL_CBASEVIDEORENDERER
#define FABLE_RETAIL_CBASEVIDEORENDERER_OVERRIDES
#include "engine/CBaseVideoRenderer.h"
long CBaseRenderer::Render(IMediaSample* pMediaSample)
{
    // If the media sample is NULL then we will have been notified by the clock that another sample is ready but in
    // the mean time someone has stopped us streaming which causes the next sample to be released
    if (pMediaSample == 0) {
        return S_FALSE;
    }
    // If we have stopped streaming then don't render any more samples
    if (m_bStreaming == 0) {
        return S_FALSE;
    }
    // Time how long the rendering takes
    OnRenderStart(pMediaSample);
    DoRenderSample(pMediaSample);
    OnRenderEnd(pMediaSample);
    return NOERROR;
}

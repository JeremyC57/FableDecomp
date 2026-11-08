// CBaseRenderer::HaveCurrentSample() -- under the renderer lock, whether a media sample is held (DirectShow BaseClasses)
// Vendor-proof: strmbase-dxsdk-sum2004(BaseClasses/renbase.cpp) ?HaveCurrentSample@CBaseRenderer@@UAEHXZ -- 41 bytes verified against Fable.exe 0x00CA4B60
// Leaf: ?HaveCurrentSample@CBaseRenderer@@UAEHXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA4B60, 41 bytes, thiscall (__fastcall this=ecx).
// Identity (vendor naming wave V1, proven): BaseClasses renbase.h declaration order: CBaseRenderer::HaveCurrentSample is primary-vtable slot 40; retail CBaseRenderer vtable 0x012BF498 slot 40 (its ??_G 0x00CA7340) and CTextureRenderer vtable 0x0129D08C (RTTI) slot 40 hold 0x00CA4B60.
//   Body: the BaseClasses source of that member (DX9-era SDK; the Win7 sample text differs only where noted), modelled on the
//   shared engine/CBaseVideoRenderer.h class (renbase.h layout) instead of a local stand-in.
//   Was: CLockedOwner_00ca4b60::HasOwner() -- under the object's critical section at +0x94, whether the pointer at +0x6c is set (Leaf none, module CLockedOwner_00ca4b60).
#define FABLE_RETAIL_CBASEVIDEORENDERER
#define FABLE_RETAIL_CBASEVIDEORENDERER_OVERRIDES
#include "engine/CBaseVideoRenderer.h"
int CBaseRenderer::HaveCurrentSample()
{
    CAutoLock cRendererLock(&m_RendererLock);
    return (m_pMediaSample == 0 ? 0 : 1);
}

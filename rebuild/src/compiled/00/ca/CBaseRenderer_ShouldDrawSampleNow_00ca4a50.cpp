// CBaseRenderer::ShouldDrawSampleNow(struct IMediaSample*, __int64*, __int64*) -- base renderer scheduling policy: never draw immediately (S_FALSE) (DirectShow BaseClasses)
// Vendor-proof: strmbase-dxsdk-sum2004(BaseClasses/renbase.cpp) ?ShouldDrawSampleNow@CBaseRenderer@@UAEJPAUIMediaSample@@PA_J1@Z -- 8 bytes verified against Fable.exe 0x00CA4A50
// Leaf: ?ShouldDrawSampleNow@CBaseRenderer@@UAEJPAUIMediaSample@@PA_J1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA4A50, 8 bytes, thiscall (__fastcall this=ecx).
// Identity (vendor naming wave V1, proven): BaseClasses renbase.h declaration order: CBaseRenderer::ShouldDrawSampleNow is primary-vtable slot 23; retail CBaseRenderer vtable 0x012BF498 slot 23 (its ??_G 0x00CA7340) and CTextureRenderer vtable 0x0129D08C (RTTI) slot 23 hold 0x00CA4A50.
//   Body: the BaseClasses source of that member (DX9-era SDK; the Win7 sample text differs only where noted), modelled on the
//   shared engine/CBaseVideoRenderer.h class (renbase.h layout) instead of a local stand-in.
//   Was: Constant-return member: returns 0x1. __fastcall this=ecx. (Leaf none, module _gapscan).
#define FABLE_RETAIL_CBASEVIDEORENDERER
#define FABLE_RETAIL_CBASEVIDEORENDERER_OVERRIDES
#include "engine/CBaseVideoRenderer.h"
long CBaseRenderer::ShouldDrawSampleNow(IMediaSample* pMediaSample, REFERENCE_TIME* ptrStart, REFERENCE_TIME* ptrEnd)
{
    return S_FALSE;
}

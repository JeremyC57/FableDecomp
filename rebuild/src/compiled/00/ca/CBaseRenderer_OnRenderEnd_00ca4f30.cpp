// CBaseRenderer::OnRenderEnd(struct IMediaSample*) -- empty hook called after each sample is rendered (renbase.cpp) (DirectShow BaseClasses)
// Leaf: ?OnRenderEnd@CBaseRenderer@@UAEXPAUIMediaSample@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA4F30, 3 bytes, thiscall (__fastcall this=ecx).
// Identity (vendor naming wave V1, proven): BaseClasses renbase.h declaration order: CBaseRenderer::OnRenderEnd is primary-vtable slot 15; retail CBaseRenderer vtable 0x012BF498 slot 15 (its ??_G 0x00CA7340) and CTextureRenderer vtable 0x0129D08C (RTTI) slot 15 hold 0x00CA4F30.
//   Body: the BaseClasses source of that member (DX9-era SDK; the Win7 sample text differs only where noted), modelled on the
//   shared engine/CBaseVideoRenderer.h class (renbase.h layout) instead of a local stand-in.
//   Was: Empty member hook. __fastcall this=ecx. (Leaf none, module _gapscan).
#define FABLE_RETAIL_CBASEVIDEORENDERER
#define FABLE_RETAIL_CBASEVIDEORENDERER_OVERRIDES
#include "engine/CBaseVideoRenderer.h"
void CBaseRenderer::OnRenderEnd(IMediaSample* pMediaSample)
{
}

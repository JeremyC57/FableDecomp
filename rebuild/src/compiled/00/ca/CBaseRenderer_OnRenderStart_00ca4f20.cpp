// CBaseRenderer::OnRenderStart(struct IMediaSample*) -- empty hook called before each sample is rendered (renbase.cpp; PERF build only does work) (DirectShow BaseClasses)
// Leaf: ?OnRenderStart@CBaseRenderer@@UAEXPAUIMediaSample@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA4F20, 3 bytes, thiscall (__fastcall this=ecx).
// Identity (vendor naming wave V1, proven): BaseClasses renbase.h declaration order: CBaseRenderer::OnRenderStart is primary-vtable slot 14; retail CBaseRenderer vtable 0x012BF498 slot 14 (its ??_G 0x00CA7340) and CTextureRenderer vtable 0x0129D08C (RTTI) slot 14 hold 0x00CA4F20.
//   Body: the BaseClasses source of that member (DX9-era SDK; the Win7 sample text differs only where noted), modelled on the
//   shared engine/CBaseVideoRenderer.h class (renbase.h layout) instead of a local stand-in.
//   Was: Empty member hook. __fastcall this=ecx. (Leaf none, module _gapscan).
#define FABLE_RETAIL_CBASEVIDEORENDERER
#define FABLE_RETAIL_CBASEVIDEORENDERER_OVERRIDES
#include "engine/CBaseVideoRenderer.h"
void CBaseRenderer::OnRenderStart(IMediaSample* pMediaSample)
{
}

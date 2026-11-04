// CBaseRenderer::GetPinCount() -- the renderer has exactly one (input) pin: returns 1 (DX9-era renbase.cpp, before the lazy-create variant) (DirectShow BaseClasses)
// Leaf: ?GetPinCount@CBaseRenderer@@UAEHXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA48F0, 6 bytes, thiscall (__fastcall this=ecx).
// Identity (vendor naming wave V1, proven): BaseClasses renbase.h declaration order: CBaseRenderer::GetPinCount is primary-vtable slot 6; retail CBaseRenderer vtable 0x012BF498 slot 6 (its ??_G 0x00CA7340) and CTextureRenderer vtable 0x0129D08C (RTTI) slot 6 hold 0x00CA48F0.
//   Body: the BaseClasses source of that member (DX9-era SDK; the Win7 sample text differs only where noted), modelled on the
//   shared engine/CBaseVideoRenderer.h class (renbase.h layout) instead of a local stand-in.
//   Was: Constant-return member: returns 0x1. __fastcall this=ecx. (Leaf none, module _gapscan).
#define FABLE_RETAIL_CBASEVIDEORENDERER
#define FABLE_RETAIL_CBASEVIDEORENDERER_OVERRIDES
#include "engine/CBaseVideoRenderer.h"
int CBaseRenderer::GetPinCount()
{
    return 1;
}

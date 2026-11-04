// CBaseRenderer::Active() -- called when the filter goes active: nothing to do (NOERROR) (DirectShow BaseClasses)
// Leaf: ?Active@CBaseRenderer@@UAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA49B0, 3 bytes, thiscall (__fastcall this=ecx).
// Identity (vendor naming wave V1, proven): BaseClasses renbase.h declaration order: CBaseRenderer::Active is primary-vtable slot 29; retail CBaseRenderer vtable 0x012BF498 slot 29 (its ??_G 0x00CA7340) and CTextureRenderer vtable 0x0129D08C (RTTI) slot 29 hold 0x00CA49B0.
//   Body: the BaseClasses source of that member (DX9-era SDK; the Win7 sample text differs only where noted), modelled on the
//   shared engine/CBaseVideoRenderer.h class (renbase.h layout) instead of a local stand-in.
//   Was: Constant-return member: returns 0. __fastcall this=ecx. (Leaf none, module _gapscan).
#define FABLE_RETAIL_CBASEVIDEORENDERER
#define FABLE_RETAIL_CBASEVIDEORENDERER_OVERRIDES
#include "engine/CBaseVideoRenderer.h"
long CBaseRenderer::Active()
{
    return NOERROR;
}

// CBaseVideoRenderer::OnWaitStart() -- empty override of the wait-start hook (renbase.cpp; PERF build only) (DirectShow BaseClasses)
// Leaf: ?OnWaitStart@CBaseVideoRenderer@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA53A0, 1 bytes, thiscall (__fastcall this=ecx).
// Identity (vendor naming wave V1, proven): BaseClasses renbase.h: CBaseVideoRenderer overrides OnWaitStart (primary slot 18); retail CBaseVideoRenderer vtable 0x012BF5D8 slot 18 (its ??_G 0x00CA73D0) holds 0x00CA53A0, CBaseRenderer vtable 0x012BF498 slot 18 holds 0x00CA6320.
//   Body: the BaseClasses source of that member (DX9-era SDK; the Win7 sample text differs only where noted), modelled on the
//   shared engine/CBaseVideoRenderer.h class (renbase.h layout) instead of a local stand-in.
//   Was: Empty member hook. __fastcall this=ecx. (Leaf none, module _gapscan).
#define FABLE_RETAIL_CBASEVIDEORENDERER
#define FABLE_RETAIL_CBASEVIDEORENDERER_OVERRIDES
#include "engine/CBaseVideoRenderer.h"
void CBaseVideoRenderer::OnWaitStart()
{
}

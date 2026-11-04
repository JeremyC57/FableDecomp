// CBaseVideoRenderer::OnWaitEnd() -- empty override of the wait-end hook (renbase.cpp; PERF build only) (DirectShow BaseClasses)
// Leaf: ?OnWaitEnd@CBaseVideoRenderer@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA53B0, 1 bytes, thiscall (__fastcall this=ecx).
// Identity (vendor naming wave V1, proven): BaseClasses renbase.h: CBaseVideoRenderer overrides OnWaitEnd (primary slot 19); retail CBaseVideoRenderer vtable 0x012BF5D8 slot 19 (its ??_G 0x00CA73D0) holds 0x00CA53B0, CBaseRenderer vtable 0x012BF498 slot 19 holds 0x00CA6330.
//   Body: the BaseClasses source of that member (DX9-era SDK; the Win7 sample text differs only where noted), modelled on the
//   shared engine/CBaseVideoRenderer.h class (renbase.h layout) instead of a local stand-in.
//   Was: Empty member hook. __fastcall this=ecx. (Leaf none, module _gapscan).
#define FABLE_RETAIL_CBASEVIDEORENDERER
#define FABLE_RETAIL_CBASEVIDEORENDERER_OVERRIDES
#include "engine/CBaseVideoRenderer.h"
void CBaseVideoRenderer::OnWaitEnd()
{
}

// CBaseFilter::GetPinVersion() -- the filter's pin version (m_PinVersion), returned for IEnumPins staleness checks (DirectShow BaseClasses)
// Leaf: ?GetPinVersion@CBaseFilter@@UAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA79F0, 4 bytes, thiscall (__fastcall this=ecx).
// Identity (vendor naming wave V1, proven): BaseClasses amfilter.h declaration order: CBaseFilter::GetPinVersion is primary-vtable slot 5; retail CBaseRenderer vtable 0x012BF498 slot 5 (its ??_G 0x00CA7340) and CTextureRenderer vtable 0x0129D08C (RTTI) slot 5 hold 0x00CA79F0.
//   Body: the BaseClasses source of that member (DX9-era SDK; the Win7 sample text differs only where noted), modelled on the
//   shared engine/CBaseVideoRenderer.h class (renbase.h layout) instead of a local stand-in.
//   Was: Member getter at this+0x48. __fastcall this=ecx. pack(1) keeps the field at (Leaf none, module _gapscan).
#define FABLE_RETAIL_CBASEVIDEORENDERER
#define FABLE_RETAIL_CBASEVIDEORENDERER_OVERRIDES
#include "engine/CBaseVideoRenderer.h"
long CBaseFilter::GetPinVersion()
{
    return m_PinVersion;
}

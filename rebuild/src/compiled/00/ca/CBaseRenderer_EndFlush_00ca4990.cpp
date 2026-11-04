// CBaseRenderer::EndFlush() -- end of a flush: reset the position pass-through's media times and let the source thread wait again (DirectShow BaseClasses)
// Leaf: ?EndFlush@CBaseRenderer@@UAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA4990, 28 bytes, thiscall (__fastcall this=ecx).
// Identity (vendor naming wave V1, proven): BaseClasses renbase.h declaration order: CBaseRenderer::EndFlush is primary-vtable slot 34; retail CBaseRenderer vtable 0x012BF498 slot 34 (its ??_G 0x00CA7340) and CTextureRenderer vtable 0x0129D08C (RTTI) slot 34 hold 0x00CA4990.
//   Body: the BaseClasses source of that member (DX9-era SDK; the Win7 sample text differs only where noted), modelled on the
//   shared engine/CBaseVideoRenderer.h class (renbase.h layout) instead of a local stand-in.
//   Was: CAppearanceOwner_00ca4990::Rebuild() -- rebuild the appearance at +0x50 when present, then this object's virtual slot 10 with 1; returns 0 (Leaf none, module CAppearanceOwner_00ca4990).
#define FABLE_RETAIL_CBASEVIDEORENDERER
#define FABLE_RETAIL_CBASEVIDEORENDERER_OVERRIDES
#include "engine/CBaseVideoRenderer.h"
class CRendererPosPassThru {                    // ctlutil.h (only the member this row calls; no shared header models it)
public:
    long ResetMediaTime();                      // 0x00CADDC0
};
long CBaseRenderer::EndFlush()
{
    // Reset the current sample media time
    if (m_pPosition) m_pPosition->ResetMediaTime();
    // There should be no outstanding advise
    SourceThreadCanWait(1);
    return NOERROR;
}

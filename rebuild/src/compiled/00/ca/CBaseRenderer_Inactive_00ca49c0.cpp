// CBaseRenderer::Inactive() -- filter going inactive: reset the position pass-through's media times, then clear the pending sample (DirectShow BaseClasses)
// Vendor-proof: strmbase-dxsdk-sum2004(BaseClasses/renbase.cpp) ?Inactive@CBaseRenderer@@UAEJXZ -- 26 bytes verified against Fable.exe 0x00CA49C0
// Leaf: ?Inactive@CBaseRenderer@@UAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA49C0, 26 bytes, thiscall (__fastcall this=ecx).
// Identity (vendor naming wave V1, proven): BaseClasses renbase.h declaration order: CBaseRenderer::Inactive is primary-vtable slot 30; retail CBaseRenderer vtable 0x012BF498 slot 30 (its ??_G 0x00CA7340) and CTextureRenderer vtable 0x0129D08C (RTTI) slot 30 hold 0x00CA49C0.
//   Body: the BaseClasses source of that member (DX9-era SDK; the Win7 sample text differs only where noted), modelled on the
//   shared engine/CBaseVideoRenderer.h class (renbase.h layout) instead of a local stand-in.
//   Was: CTextureRenderer::vfunc30() -- DirectShow renderer flush-end shape: reset the position pass-through's media times (when one is attached), call virtual slot 28 on itself, return S_OK (Leaf ?vfunc30@CTextureRenderer, module CTextureRenderer).
#define FABLE_RETAIL_CBASEVIDEORENDERER
#define FABLE_RETAIL_CBASEVIDEORENDERER_OVERRIDES
#include "engine/CBaseVideoRenderer.h"
class CRendererPosPassThru {                    // ctlutil.h (only the member this row calls; no shared header models it)
public:
    long ResetMediaTime();                      // 0x00CADDC0
};
long CBaseRenderer::Inactive()
{
    if (m_pPosition) {
        m_pPosition->ResetMediaTime();
    }
    // People who derive from this may want to override this behaviour
    // to keep hold of the sample in some circumstances
    ClearPendingSample();
    return NOERROR;
}

// CEngineScreenEffectColourFilter::Render(const CEngineComponentRenderParams&) -- apply the colour filter when the component renders and
//   the filter's alpha is above DELTA_NOTIONAL_ZERO
// Leaf: ?Render@CEngineScreenEffectColourFilter@@UAEXABVCEngineComponentRenderParams@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B5AF60 (CEngineScreenEffectColourFilter vtable 0x012A25CC, the CEngineComponent::Render slot).
// Names (NF201, 2026-11-20): own symbol Ego_r.pdb 0x00A007F0 + FableWin.pdb 0x02D26590
//   ?Render@CEngineScreenEffectColourFilter@@UAEXABVCEngineComponentRenderParams@@@Z (public virtual, one const params& argument);
//   callee Ego_r.pdb 0x009FFCB0 + FableWin.pdb 0x02D25E80 ?ApplyColourFilter@CEngineScreenEffectColourFilter@@AAEXXZ (0x00B5A3F0, private,
//   no arguments; was the free placeholder CEngineScreenEffectColourFilter_DoRender). Reads CEngineComponent::RenderEnabled +0x08 through
//   FableWin.pdb's inline ?PeekRenderEnabled@CEngineComponent@@QBE_NXZ and Settings.Alpha (CEngineColourFilterSettings +0x00, so +0x10).
//   Was a free __fastcall placeholder over a local pad model of the class.
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
#define FABLE_RETAIL_CENGINESCREENEFFECTCOLOURFILTER
#define FABLE_RETAIL_CENGINESCREENEFFECTCOLOURFILTER_MEMBERS
#define FABLE_RETAIL_CENGINESCREENEFFECTCOLOURFILTER_RENDER
#include "engine/CEngineScreenEffectColourFilter.h"

extern const float DELTA_NOTIONAL_ZERO;                 // 0x0129BA3C

void CEngineScreenEffectColourFilter::Render(const CEngineComponentRenderParams& params)
{
    if (PeekRenderEnabled() && Settings.Alpha > DELTA_NOTIONAL_ZERO)
        ApplyColourFilter();
}

// CGameScriptInterface::RadialBlurFadeOut(float time, void* handle) const -- forward to the environment's spell-effect interpolator
// (world +0x18 -> +0x1964), the ScreenFilterFadeOut 0x00890200 shape
// Leaf: ?RadialBlurFadeOut@CGameScriptInterface@@UBEXMPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00890150 (vtable slot 669): loads World +0x04 -> +0x18 -> +0x1964 and tail-forwards (time, handle)
//   to CEnvironmentSpellEffectInterpolator::RadialBlurFadeOut 0x006B82E0.
// Names (NF197, 2026-11-20): own symbol Ego_r.pdb 0x008CCF40 + FableWin.pdb 0x02A74C90 ?RadialBlurFadeOut@CGameScriptInterface@@UBEXMPAX@Z
//   (engine/CGameScriptInterface.h slot 669 declares it at this address); callee Ego_r.pdb 0x006F4280 + FableWin.pdb 0x0246CEA0
//   ?RadialBlurFadeOut@CEnvironmentSpellEffectInterpolator@@QAEXMPAX@Z. Was a free __fastcall stand-in calling Tgt::method(int, int)
//   over a local CWorld model; re-authored on the landed sibling 0x00890200's text (its world-chain overlays).
#define FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_RADIALBLURFADEOUT
#include "engine/CEnvironmentSpellEffectInterpolator.h"

struct Field18Overlay
{
    char m_Pad00[0x1964];
    CEnvironmentSpellEffectInterpolator* m_pSpellEffectInterpolator;
};

struct Field04Overlay
{
    char m_Pad00[0x18];
    Field18Overlay* m_Field18;
};

struct SelfOverlay
{
    char m_Pad00[0x04];
    Field04Overlay* m_Field04;
};

#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::RadialBlurFadeOut(float time, void* handle) const
{
    const SelfOverlay* self = (const SelfOverlay*)this;
    Field04Overlay* field04 = self->m_Field04;
    Field18Overlay* field18 = field04->m_Field18;
    field18->m_pSpellEffectInterpolator->RadialBlurFadeOut(time, handle);
}

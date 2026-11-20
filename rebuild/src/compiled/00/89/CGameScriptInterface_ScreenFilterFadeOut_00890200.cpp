// Leaf: ?ScreenFilterFadeOut@CGameScriptInterface@@UBEXMPAX@Z
// NF122 (2026-11-20, q6936): the jump target 0x006B83F0 is CEnvironmentSpellEffectInterpolator::ScreenFilterFadeOut(float, void*)
//   (Ego_r.pdb 0x006F4390 + FableWin.pdb 0x0246D190; the environment's +0x1964 member is the spell-effect interpolator), declared by
//   engine/CEnvironmentSpellEffectInterpolator.h (FADEOUT); was the stand-in CXMVPlaybackInfo::FadeOut.
#define FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_FADEOUT
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

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::ScreenFilterFadeOut(float fadeOutTime, void* pFadeOutContext) const
{
    const SelfOverlay* self = (const SelfOverlay*)this;
    Field04Overlay* field04 = self->m_Field04;
    Field18Overlay* field18 = field04->m_Field18;
    field18->m_pSpellEffectInterpolator->ScreenFilterFadeOut(fadeOutTime, pFadeOutContext);
}
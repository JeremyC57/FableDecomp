// CGameScriptInterface::RadialBlurFadeOut(float time, void* handle) const -- forward to the environment's spell-effect interpolator
// (World +0x04 -> GetEnvironment() +0x18 -> GetSpellEffectInterpolator() +0x1964), the ScreenFilterFadeOut 0x00890200 shape
// Leaf: ?RadialBlurFadeOut@CGameScriptInterface@@UBEXMPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00890150 (CGameScriptInterface vtable 0x01260F0C slot 669): loads World +0x04 -> +0x18 -> +0x1964
//   and tail-forwards (time, handle) to CEnvironmentSpellEffectInterpolator::RadialBlurFadeOut 0x006B82E0.
// Names (NF197 / NF201, 2026-11-20): own symbol Ego_r.pdb 0x008CCF40 + FableWin.pdb 0x02A74C90
//   ?RadialBlurFadeOut@CGameScriptInterface@@UBEXMPAX@Z (engine/CGameScriptInterface.h slot 669 declares it at this address); callee
//   Ego_r.pdb 0x006F4280 + FableWin.pdb 0x0246CEA0 ?RadialBlurFadeOut@CEnvironmentSpellEffectInterpolator@@QAEXMPAX@Z; the chain through
//   the FableWin inline accessors ?GetEnvironment@CWorld@@QAEAAVCEnvironment@@XZ and
//   ?GetSpellEffectInterpolator@CEnvironment@@QAEAAVCEnvironmentSpellEffectInterpolator@@XZ (the CTCBulletTime::Pause 0x0079CF30 recipe).
//   Was a free __fastcall stand-in calling Tgt::method(int, int) over a local CWorld model.
#include "fable_compiler.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_SPELLEFFECTINTERPOLATOR
#define FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_RADIALBLURFADEOUT
#include "engine/CGameScriptInterface.h"
#include "engine/CWorld.h"
#include "engine/CEnvironment.h"
#include "engine/CEnvironmentSpellEffectInterpolator.h"

void CGameScriptInterface::RadialBlurFadeOut(float time, void* handle) const
{
    World->GetEnvironment().GetSpellEffectInterpolator().RadialBlurFadeOut(time, handle);
}

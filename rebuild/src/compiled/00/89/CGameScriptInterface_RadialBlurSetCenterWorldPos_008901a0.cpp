// CGameScriptInterface::RadialBlurSetCenterWorldPos(void* key, const C3DVector& pos) const -- the world environment's spell-effect interpolator, a tail jump
// Leaf: ?RadialBlurSetCenterWorldPos@CGameScriptInterface@@UBEXPAXABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008901A0. Identity (NF431, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: ?GetEnvironment@CWorld@@QAEAAVCEnvironment@@XZ (inline: World +0x18), then the inline
//   ?GetSpellEffectInterpolator@CEnvironment@@QAEAAVCEnvironmentSpellEffectInterpolator@@XZ (+0x1964), then
//   ?RadialBlurSetCenterWorldPos@CEnvironmentSpellEffectInterpolator@@QAEXPAXABVC3DVector@@@Z (retail 0x006B8310, the jmp target).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CENVIRONMENT_SPELLEFFECTINTERPOLATOR
#define FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_SCRIPTFORWARDS
#include "engine/CGameScriptInterface.h"
#include "engine/CWorld.h"
#include "engine/CEnvironment.h"
#include "engine/CEnvironmentSpellEffectInterpolator.h"

void CGameScriptInterface::RadialBlurSetCenterWorldPos(void* key, const C3DVector& pos) const
{
    World->PEnvironment_ptr->GetSpellEffectInterpolator().RadialBlurSetCenterWorldPos(key, pos);
}

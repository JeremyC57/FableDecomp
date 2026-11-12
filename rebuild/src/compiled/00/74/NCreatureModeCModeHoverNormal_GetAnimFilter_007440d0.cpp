// NCreatureMode::CModeHoverNormal::GetAnimFilter() const -- returns the mode's own animation filter (Filter)
// Leaf: ?GetAnimFilter@CModeHoverNormal@NCreatureMode@@UBEABVCAnimFilterBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007440D0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 4 of NCreatureMode::CModeHoverNormal's vtable 0x012687FC (the slot holds this body in 1 vtable(s), owner NCreatureMode::CModeHoverNormal); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts GetAnimFilter(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetAnimFilter@CModeHoverNormal@NCreatureMode@@UBEABVCAnimFilterBase@@XZ; `ret` matches the signature.
// Was: NCreatureMode::CModeHoverNormal::vfunc_4 (rebuild/src/compiled/00/74/gapscan_sub007440d0_007440d0.cpp).
// No shared header for NCreatureMode::CModeHoverBase: minimal local definition (RTTI base CTCCreatureModeManager::CModeBase).
// Ego_r.pdb members HoverStates (vector) +0x1c, CurrentName, PCurrentState, CurrentMovementType, AnimQueue (list), PParticleEffect,
// size 0x4c; retail is 0x40 (the derived Filter at +0x40, `lea eax,[ecx+40h]`): the STLport vector / list are smaller in retail
// (opaque here).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CAnimFilterOfGroup.h"
namespace NCreatureMode {
class CModeHoverBase : public CTCCreatureModeManager::CModeBase {
public:
    CModeHoverBase(CThing& thing);
protected:
    virtual CCharString GetEffectHelperPointName() const;
    unsigned char HoverStatesToPParticleEffect[0x24];                    // +0x1c HoverStates..PParticleEffect (opaque)
};
class CModeHoverNormal : public CModeHoverBase {
public:
    CModeHoverNormal(CThing& thing);
    virtual const CAnimFilterBase& GetAnimFilter() const;
private:
    CAnimFilterOfGroup Filter;                                           // +0x40
};
const CAnimFilterBase& CModeHoverNormal::GetAnimFilter() const
{
    return Filter;
}
}

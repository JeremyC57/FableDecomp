// NCreatureMode::CModeHoverBase::GetEffectHelperPointName() const -- the base hover mode has no effect helper point: returns an empty name
// Leaf: ?GetEffectHelperPointName@CModeHoverBase@NCreatureMode@@MBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00743DF0, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 15 of NCreatureMode::CModeHoverBase's vtable 0x01287D14 (the slot holds this body in 3 vtable(s), owner NCreatureMode::CModeHoverBase); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts GetEffectHelperPointName(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetEffectHelperPointName@CModeHoverBase@NCreatureMode@@MBE?AVCCharString@@XZ; `ret` matches the signature.
// Was: NCreatureMode::CModeHoverBase::vfunc_15 (rebuild/src/compiled/00/74/CActionDoCreatureAction_GetActionName_00743df0.cpp).
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
namespace NCreatureMode {
class CModeHoverBase : public CTCCreatureModeManager::CModeBase {
public:
    CModeHoverBase(CThing& thing);
protected:
    virtual CCharString GetEffectHelperPointName() const;
    unsigned char HoverStatesToPParticleEffect[0x24];                    // +0x1c HoverStates..PParticleEffect (opaque)
};
CCharString CModeHoverBase::GetEffectHelperPointName() const
{
    return CCharString("");
}
}

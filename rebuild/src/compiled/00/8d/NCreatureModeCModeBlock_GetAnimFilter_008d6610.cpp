// NCreatureMode::CModeBlock::GetAnimFilter() const -- returns the weapon-based filter it inherits from CModeBase (WeaponBasedFilter, opaque in the shared header)
// Leaf: ?GetAnimFilter@CModeBlock@NCreatureMode@@UBEABVCAnimFilterBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D6610, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 4 of NCreatureMode::CModeBlock's vtable 0x01267C3C (the slot holds this body in 1 vtable(s), owner NCreatureMode::CModeBlock); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetAnimFilter(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetAnimFilter@CModeBlock@NCreatureMode@@UBEABVCAnimFilterBase@@XZ; `ret` matches the signature.
// Was: NCreatureMode::CModeBlock::vfunc_4 (rebuild/src/compiled/00/8d/gapscan_sub008d6610_008d6610.cpp).
// No shared header for NCreatureMode::CModeBlock: minimal local definition (RTTI base CTCCreatureModeManager::CModeBase; Ego_r.pdb
// PCreature +0x1c, size 0x20). The body reads CModeBase::WeaponBasedFilter (+0x8, `lea eax,[ecx+8]`).
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
class CThingCreatureBase;
namespace NCreatureMode {
class CModeBlock : public CTCCreatureModeManager::CModeBase {
public:
    CModeBlock(CThing& thing);
    virtual const CAnimFilterBase& GetAnimFilter() const;
private:
    CThingCreatureBase* PCreature;                                       // +0x1c
};
const CAnimFilterBase& CModeBlock::GetAnimFilter() const
{
    return reinterpret_cast<const CAnimFilterBase&>(WeaponBasedFilter);
}
}

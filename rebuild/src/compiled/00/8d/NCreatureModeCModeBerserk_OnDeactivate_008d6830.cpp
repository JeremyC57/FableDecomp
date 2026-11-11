// NCreatureMode::CModeBerserk::OnDeactivate() -- leaving the mode: the stand-mode deactivation (tail call to CModeStandBase::OnDeactivate 0x00861D00)
// Leaf: ?OnDeactivate@CModeBerserk@NCreatureMode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D6830, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 6 of NCreatureMode::CModeBerserk's vtable 0x01267F00 (the slot holds this body in 1 vtable(s), owner NCreatureMode::CModeBerserk); the Xbox 360 Test PDB virtual order (third source) aligned on 6 TLC-named slots of that vtable puts OnDeactivate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnDeactivate@CModeBerserk@NCreatureMode@@UAEXXZ; no `ret` to check (NORET).
// Was: NCreatureMode::CModeBerserk::vfunc_6 (rebuild/src/compiled/00/8d/gapscan_sub008d6830_008d6830.cpp).
// No shared header for NCreatureMode::CModeBerserk: minimal local definition (RTTI base CModeStandBase), only the override.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CMODESTANDBASE_DEACTIVATE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CModeStandBase.h"
namespace NCreatureMode {
class CModeBerserk : public CModeStandBase {
public:
    CModeBerserk(CThing& thing);
    virtual void OnDeactivate();
};
void CModeBerserk::OnDeactivate()
{
    CModeStandBase::OnDeactivate();
}
}

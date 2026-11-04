// CAIStateGroup_MazeShieldedChase::IsStillRunnable() -- runnable while the target is alive and the group is not stopping and the creature still has its physical shield; when the shield is gone the will-power indicator is reset to -1
// Leaf: ?IsStillRunnable@CAIStateGroup_MazeShieldedChase@@IAE_NXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00954740, 136 bytes; Ego_r.pdb ?IsStillRunnable@CAIStateGroup_MazeShieldedChase@@IAE_NXZ
//   (protected, non-virtual).
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (twice, IS_THING_ALIVE), CAIStateGroupBase::IsStopping() const
//   0x008FD010, LowerBound 0x0040F020 (CThing::GetTC<CTCAICreatureWillPowerIndicator> through Components.Find),
//   CTCAICreatureWillPowerIndicator::Reset(float) 0x007E0E70.
// Shape: the old draft's this/esi v &PTargetThing/edi register swap came from local CThing / pointer models; on the retail CThing,
//   CIntelligentPointer, CThingAICreature and CAIStateGroupBase headers VC7.1 keeps `this` in edi like retail.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"

class CTCAICreatureWillPowerIndicator : public CTCBase {            // no shared header
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_AICREATURE_WILL_POWER_INDICATOR; }
    void Reset(float value);                                        // 0x007E0E70
};
template<class T> inline bool IS_THING_ALIVE(const CIntelligentPointer<T>& p) { return p.Get() != 0 && !p.Get()->IsDead(); }

class CAIStateGroup_MazeShieldedChase : public CAIStateGroupBase {
protected:
    bool IsStillRunnable();
    CIntelligentPointer<CThingCreatureBase> PTargetThing;           // +0x14
};

bool CAIStateGroup_MazeShieldedChase::IsStillRunnable()
{
    if (!IS_THING_ALIVE(PTargetThing) || IsStopping())
        return false;
    CThingAICreature* creature = PBrain->Creature;
    if (!creature->HasTC(TCI_PHYSICAL_SHIELD_SPELL)) {
        CTCAICreatureWillPowerIndicator* indicator;
        if (creature->GetTC(&indicator))
            indicator->Reset(-1.0f);
        return false;
    }
    return true;
}
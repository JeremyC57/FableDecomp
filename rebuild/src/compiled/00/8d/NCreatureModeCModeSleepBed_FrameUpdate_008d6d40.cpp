// NCreatureMode::CModeSleepBed::FrameUpdate() -- the CModeBase frame update (0x0073AC50), then, while the mode holds a pending remove action
//   (PRemoveAction +0x2C), the creature takes it when it is free to (IsFreeToPerformAction 0x00662210, SetCurrentAction(const CCountedPointer&)
//   0x00664670)
// Leaf: ?FrameUpdate@CModeSleepBed@NCreatureMode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D6D40, 45 bytes; vtable slot 3 of NCreatureMode::CModeSleepBed. The same text as the landed sibling
//   CModeSitBase::FrameUpdate 0x008D6C80 (NF83) over CModeSleepBed's own PRemoveAction.
// Names: Ego_r.pdb 0x008180C0 ?FrameUpdate@CModeSleepBed@NCreatureMode@@UAEXXZ; members Ego_r.pdb NCreatureMode::CModeSleepBed (52 bytes:
//   Filter, SleepAnimName, PRemoveAction at retail = Ego_r offsets).
// Port (NF86, round 7, q11114 binder): was a namespaced local model (NCreatureMode::CThingCreatureBase / CModeBase / CCountedActionPointer)
//   binding 0x00662210 as ?IsFreeToPerformAction@CThingCreatureBase@NCreatureMode@@QAE_NPAUCCreatureActionBase@2@@Z; now the shared
//   CThingCreatureBase.h (PDB IsFreeToPerformAction + NF83 SETCURRENTACTION_COUNTED), CTCCreatureModeManager.h (CModeBase), CCountedPointer.h
//   and CAnimFilterOfGroup.h; CModeSleepBed itself has no shared header, so it is modelled here on those shared types (the 0x008D6C80 body).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_SETCURRENTACTION_COUNTED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCountedPointer.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CAnimFilterOfGroup.h"

namespace NCreatureMode {
// No shared header for this class: Ego_r.pdb NCreatureMode::CModeSleepBed (52 bytes): CTCCreatureModeManager::CModeBase, Filter
// CAnimFilterOfGroup +0x1C, SleepAnimName CCharString +0x28, PRemoveAction CCountedPointer<CCreatureActionBase> +0x2C (Ego_r = retail offsets).
class CModeSleepBed : public CTCCreatureModeManager::CModeBase {
public:
    virtual void FrameUpdate();                                          // slot 3, 0x008D6D40
private:
    CAnimFilterOfGroup Filter;                                           // +0x1C
    CCharString SleepAnimName;                                           // +0x28
    CCountedPointer<CCreatureActionBase> PRemoveAction;                  // +0x2C
};
FABLE_STATIC_ASSERT(sizeof(CModeSleepBed) == 0x34);

void CModeSleepBed::FrameUpdate()
{
    CModeBase::FrameUpdate();
    if (PRemoveAction.Get()) {
        CThingCreatureBase& creature = static_cast<CThingCreatureBase&>(ParentThing);
        if (creature.IsFreeToPerformAction(*PRemoveAction))
            creature.SetCurrentAction(PRemoveAction);
    }
}
}

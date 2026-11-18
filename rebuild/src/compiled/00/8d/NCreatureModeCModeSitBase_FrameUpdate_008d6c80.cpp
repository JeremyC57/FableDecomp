// NCreatureMode::CModeSitBase::FrameUpdate() -- the CModeBase frame update (0x0073AC50), then, while the mode holds its remove action
// (PRemoveAction, +0x24), the parent creature takes it when it is free to (IsFreeToPerformAction 0x00662210 with the action, then
// SetCurrentAction 0x00664670 with the counted pointer)
// Leaf: ?FrameUpdate@CModeSitBase@NCreatureMode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D6C80, 45 bytes, thiscall `ret`.
// Identity: Ego_r.pdb 0x00818010 / FableWin.pdb 0x02842320 ?FrameUpdate@CModeSitBase@NCreatureMode@@UAEXXZ (neither PDB has a
//   CModeSitChair::FrameUpdate: CModeSitChair's vtable slot 3 holds the inherited base override); the +0x24 member is Ego_r.pdb
//   CModeSitBase::PRemoveAction (CCountedPointer<CCreatureActionBase>), the +4 word CModeBase::ParentThing.
// Callees: CModeBase::FrameUpdate 0x0073AC50, CThingCreatureBase::IsFreeToPerformAction 0x00662210 (QBE_NABV, both PDBs),
//   CThingCreatureBase::SetCurrentAction(const CCountedPointer<CCreatureActionBase>&) 0x00664670 (both PDBs).
// NF83 (round 7, q11019 binder): ported onto engine/NCreatureMode_CModeSitFloor.h / engine/CTCCreatureModeManager.h /
//   engine/CThingCreatureBase.h; the namespaced stand-ins NCreatureMode::CThingCreatureBase / CCreatureActionBase /
//   CCountedActionPointer / CModeBase / CModeSitChair are gone. NF83 overlay opt-ins: NCREATUREMODE_CMODESITBASE_FRAMEUPDATE and
//   CTHINGCREATUREBASE_SETCURRENTACTION_COUNTED.
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
#define FABLE_RETAIL_NCREATUREMODE_CMODESITBASE_FRAMEUPDATE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/NCreatureMode_CModeSitFloor.h"

void NCreatureMode::CModeSitBase::FrameUpdate()
{
    CModeBase::FrameUpdate();
    if (PRemoveAction.Get()) {
        CThingCreatureBase& creature = static_cast<CThingCreatureBase&>(ParentThing);
        if (creature.IsFreeToPerformAction(*PRemoveAction))
            creature.SetCurrentAction(PRemoveAction);
    }
}

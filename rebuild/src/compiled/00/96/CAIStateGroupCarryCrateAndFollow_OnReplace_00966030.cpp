// CAIStateGroup_CarryCrateAndFollow::OnReplace() -- release the crate's action point, stop the pick-up action, drop the crate and put the previously carried item back in its slot, then clear the action point state and the remembered item
// Leaf: ?OnReplace@CAIStateGroup_CarryCrateAndFollow@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00966030, 597 bytes, plain `ret` (vtable slot 10; Ego_r.pdb name above).
// Callees: IS_THING_ALIVE / CIntelligentPointer (GetPItem 0x00A01B50), CThing::GetTC<T> (expanded; CVectorMap::LowerBound 0x0040F020)
//   for CTCSingleUserActionPoints / CTCCreatureModeManager / CTCCarrying, CTCUserActionPointBase::SetActionPointAsFree 0x007FEF40,
//   CThingCreatureBase::IsPerformingAnyAction 0x006622A0, CCreatureActionBase::GetActionID 0x00692620,
//   CThingCreatureBase::FinishCurrentAction 0x00662650, CTCCarrying::IsCarryingThing(const CThing&) 0x006A1060 / RemoveThing 0x006A19B0 /
//   GetThingInCarrySlot 0x006A11F0 / RemoveThingInCarrySlot 0x006A16A0 / PutThingInCarrySlot 0x006A1F80,
//   CTCCreatureModeManager::RemoveMode 0x0073ABA0 / AddMode 0x0073AE10, CDefClassBase::GetSubDef<CCarryableDef> 0x005CC10E,
//   the CIntelligentPointer<CThing>(0) temporary (0x00A01B10 / 0x00A01B90 / 0x00A01C10) assigned to PPreviouslyCarriedItem.
// Shape: on the shared headers (the earlier local model kept this in ebp and &PCrate in ebx; the port gives retail's ebx / ebp).
//   GetSubDef<CCarryableDef>'s body is visible but out of line (CDefClassBase_GetSubDef.h, FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE):
//   VC7.1 then knows the def's address is not kept and drops the release's null test (0x00966227 `dec dword ptr [ecx+4]`). The
//   passive-slot occupant is a named local compared first (`cmp ebx,eax`, 0x009661E9; the one-expression form calls GetPItem first).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_GETACTIONID
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_SETCOMBATANIMATION
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CTCCARRYING_PUTTHING
#define FABLE_RETAIL_CTCCARRYING_REMOVETHING
#define FABLE_RETAIL_CTCCARRYING_SLOTACCESS
#define FABLE_RETAIL_CTCCARRYING_CARRYSLOT
#define FABLE_RETAIL_CCARRYABLEDEF
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_CARRYCRATEANDFOLLOW
#include "rebuild_abi.h"
#include "engine/ETCInterfaceType.h"
#include "engine/NCreatureMode.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBaseDef.h"
#include "engine/CCarryableDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/campfire_types.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_CarryCrateAndFollow.h"

void CAIStateGroup_CarryCrateAndFollow::OnReplace()
{
    if (IS_THING_ALIVE(PCrate) && GotActionPoint) {
        CTCSingleUserActionPoints* action_points;
        PCrate->GetTC(&action_points);
        action_points->SetActionPointAsFree(ActionPointIndex);
        if (PickUpActionID != -1 && GetCreature().IsPerformingAnyAction()
            && GetCreature().PeekPCurrentAction()->GetActionID() == PickUpActionID)
            GetCreature().FinishCurrentAction();
        CThingAICreature& creature = GetCreature();
        CTCCreatureModeManager* modes;
        creature.GetTC(&modes);
        CTCCarrying* carrying;
        creature.GetTC(&carrying);
        if (carrying->IsCarryingThing(*PCrate)) {
            carrying->RemoveThing(*PCrate);
            modes->RemoveMode(NCreatureMode::MODE_CARRYING_GENERIC_BOX);
        }
        if (IS_THING_ALIVE(PPreviouslyCarriedItem)) {
            CDefPointer<const CCarryableDef> carryable_def;
            PPreviouslyCarriedItem->PeekBaseDef().GetSubDef(&carryable_def);
            long passive_slot = carryable_def->PassiveCarrySlot;
            long active_slot = carryable_def->ActiveCarrySlot;
            CThing* slot_thing = carrying->GetThingInCarrySlot(passive_slot);
            if (slot_thing == PPreviouslyCarriedItem.Get())
                carrying->RemoveThingInCarrySlot(passive_slot, false);
            carrying->PutThingInCarrySlot(*PPreviouslyCarriedItem, active_slot, true);
            if (PPreviouslyCarriedItem->HasTC(TCI_WEAPON))
                modes->AddMode(NCreatureMode::MODE_CARRYING_WEAPON_STAND);
        }
    }
    ActionPointIndex = -1;
    GotActionPoint = false;
    PickUpActionID = -1;
    PPreviouslyCarriedItem = CIntelligentPointer<CThing>(0);
}
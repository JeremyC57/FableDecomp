// CAIStateGroup_RecoverStealableItem::Update() -- claim a free action point on the stolen item, walk to it (or near the item when the
// point cannot be reached), face it, pick it up (killing a previously carried weapon), carry it back to the stealable-item location
// and drop it there; any failure (item gone or dead, someone else carrying it, an action not finishing) terminates the state group.
// Leaf: ?Update@CAIStateGroup_RecoverStealableItem@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009658B0, 1276 bytes.
// Callees (Ego_r.pdb / FableWin.pdb spellings): CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap LowerBound 0x0040F020 /
//   0x004365B0 (const), CTCUserActionPointBase::GetFreeActionPoint 0x007FEDE0, CThing::GetPos 0x004C73D0,
//   IS_THING_ALIVE<CIntelligentPointer<CThing> > 0x004CC340 / <CIntelligentPointer<const CThing> > 0x0069CE20,
//   CAIStateGroupBase::AIAction_MoveToPos 0x0097EC00 / AIAction_MoveToPosProximity 0x0097E890 / AIAction_LookAtPos 0x0097D580 /
//   AIAction_PerformActionAndWait 0x0097DB70 / Terminate 0x008FCF40, CCreatureAction_PickUpGenericBox ctor 0x00843700 (implicit dtor
//   COMDAT 0x007120D0), CCreatureActionBase::GetActionID 0x00692620, CTCCarrying::GetThingInPrimarySlot 0x006A1440 /
//   RemoveThingFromPrimarySlot 0x006A1E00 / IsCarryingThing 0x006A1060, CIntelligentPointer<CThing>(CThing*) 0x004A96F0 / operator=
//   0x004A9760, CThing::Kill 0x004C9B80, CCreatureAction_DropGenericBox ctor 0x00842150, ~CCreatureActionBase 0x00693EF0.
// Components: TCI_SINGLE_USER_ACTION_POINTS (45, CTCUserActionPointBase), TCI_CARRYABLE (71, PCarryingTC), TCI_CARRYING (70),
//   TCI_WEAPON (72), TCI_STEALABLE_ITEM_LOCATION (178, CTCStealableItemLocation::RadiusToTakeItemsBackTo +0x10; no shared header:
//   a local model with its Ego_r.pdb members).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_OUTOFLINE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_GETACTIONID
#define FABLE_RETAIL_CCREATUREACTION_GENERICBOX
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_SETCOMBATANIMATION
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_CTCCARRYING_REMOVETHING
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAISTATEGROUP_RECOVERSTEALABLEITEM
#define FABLE_RETAIL_CAISTATEGROUP_RECOVERSTEALABLEITEM_UPDATE
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCCarryable.h"
#include "engine/CCreatureAction_PickUpGenericBox.h"
#include "engine/CCreatureAction_DropGenericBox.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_RecoverStealableItem.h"
#include "engine/ThingAlive.h"
#include "engine/campfire_types.h"

class CTCStealableItemLocation : public CTCBase {            // TCI_STEALABLE_ITEM_LOCATION; Ego_r.pdb members
public:
    float RadiusToBeWithin;                                  // +0x0c
    float RadiusToTakeItemsBackTo;                           // +0x10
};

void CAIStateGroup_RecoverStealableItem::Update()
{
    if (!PStealableItem.Get() || PStealableItem->IsDead()) {
        Terminate();
        return;
    }
    CTCUserActionPointBase* ptc_action_points;
    PStealableItem->GetTC(TCI_SINGLE_USER_ACTION_POINTS, &ptc_action_points);
    C3DVector pos;
    CRightHandedSet rh_set;
    if (!ptc_action_points->GetFreeActionPoint(GetCreature(), pos, rh_set, ActionPointIndex)) {
        Terminate();
        return;
    }
    GotActionPoint = true;
    C3DVector face_pos = rh_set.GetForward();
    CTCCarryable* ptc_carryable;
    PStealableItem->GetTC(TCI_CARRYABLE, &ptc_carryable);
    bool arrived;
    EAIActionReturn result;
    do {
        if (!PStealableItem.Get() || PStealableItem->IsDead()) {
            Terminate();
            return;
        }
        if (ptc_carryable->PCarryingTC != 0) {
            Terminate();
            return;
        }
        bool reached;
        result = AIAction_MoveToPos((EAIActionRepeat)2, pos, reached, (EMovementType)5, true, true, 0, true, 0.0f);
    } while (result == 4);
    if (result == 3) {
        do {
            if (!PStealableItem.Get() || PStealableItem->IsDead()) {
                Terminate();
                return;
            }
            if (ptc_carryable->PCarryingTC != 0) {
                Terminate();
                return;
            }
            bool reached;
            result = AIAction_MoveToPosProximity((EAIActionRepeat)2, PStealableItem->GetPos(), 1.0f, reached, (EMovementType)5, true, false, true, 0, true, 0.0f);
        } while (result == 4);
        if (IS_THING_ALIVE(PStealableItem))
            face_pos = PStealableItem->GetPos() - GetCreature().GetPos();
    }
    if (!IS_THING_ALIVE(PStealableItem) || ptc_carryable->PCarryingTC != 0 || result != 2) {
        Terminate();
        return;
    }
    result = AIAction_LookAtPos((EAIActionRepeat)1, GetCreature().GetPos() + face_pos, 0x21, 0.0027777f);
    if (!IS_THING_ALIVE(PStealableItem) || ptc_carryable->PCarryingTC != 0 || result != 2) {
        Terminate();
        return;
    }
    CCreatureAction_PickUpGenericBox pick_up(GetCreature(), *PStealableItem);
    PickUpActionID = pick_up.GetActionID();
    CTCCarrying* ptc_carrying;
    GetCreature().GetTC(TCI_CARRYING, &ptc_carrying);
    {
        CIntelligentPointer<CThing> previous(ptc_carrying->GetThingInPrimarySlot());
        PPreviouslyCarriedItem = previous;
    }
    ptc_carrying->RemoveThingFromPrimarySlot(false);
    if (IS_THING_ALIVE(PPreviouslyCarriedItem) && PPreviouslyCarriedItem->HasTC(TCI_WEAPON))
        PPreviouslyCarriedItem->Kill(true);
    result = AIAction_PerformActionAndWait(pick_up, false);
    if (IS_THING_ALIVE(PTargetPoint) && IS_THING_ALIVE(PStealableItem) && result == 2 && ptc_carrying->IsCarryingThing(*PStealableItem)) {
        face_pos = PTargetPoint->GetPos();
        const CTCStealableItemLocation* ptc_location;
        PTargetPoint->PeekTC(TCI_STEALABLE_ITEM_LOCATION, &ptc_location);
        float radius = ptc_location->RadiusToTakeItemsBackTo;
        if (AIAction_MoveToPosProximity((EAIActionRepeat)1, face_pos, radius, arrived, (EMovementType)5, true, false, true, 0, true, 0.0f) == 2) {
            ReachedDestination = true;
            AIAction_PerformActionAndWait(CCreatureAction_DropGenericBox(GetCreature()), false);
        }
    }
    Terminate();
}
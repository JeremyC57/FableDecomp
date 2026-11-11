// CAIStateGroup_TavernServer::Update() -- the tavern server's work loop: find a serving point, fetch and pick up a jug from the bar, walk to the serving point and fill tankards until the jug is empty (freeing the point when a trip fails), then take the jug back to its old bar point and put it down; terminate when anything is missing
// Leaf: ?Update@CAIStateGroup_TavernServer@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00967D40, 760 bytes (768 with padding); thiscall; CAIStateGroup_TavernServer vtable slot 12
//   (0x0128CD44, the CAIStateGroupBase::Update pure-virtual slot).
// Identity: Ego_r.pdb / FableWin.pdb ?Update@CAIStateGroup_TavernServer@@MAEXXZ (protected virtual; Ego_r 0x0098C7C0, the last body of
//   the class after FillTankard / WalkToBar / WalkToServingPoint, as retail's 0x00967D40 follows FillTankard 0x00967B40).
// Callees: CTCVillageMember::GetPVillage 0x007EE540 (GetTC<CTCVillageMember>, interface 0x23, through CVectorMap::LowerBound 0x0040F020),
//   the same-TU CAIStateGroup_TavernServer members GetServingPoint 0x00966DA0, IsCarryingJug 0x00966CF0, GetJug 0x00967180,
//   WalkToPoint 0x009677A0, PickUpJug 0x00967930, FillTankard 0x00967B40, GetOldJugPoint 0x009672C0, PutDownJug 0x00967A20;
//   CAIStateGroupBase::IsStopping 0x008FD010 / AIAction_DoNothing 0x0097E200 / AIAction_MinionWander 0x0097F870 / Terminate 0x008FCF40;
//   CTCTavernTable::FreeServingPoint(long) 0x007B9090; CBaseIntelligentPointer::GetPItem 0x00A01B50.
// Shape: WanderWithinTavern (0x00966AD0), FreeServingPoint (0x009670A0), WalkToBar and WalkToServingPoint are same-TU bodies VC7.1
//   expands here; in the loops the CVectorMap lookups of GetTC<CTCTavernJug> / GetTC<CTCTavernTable> expand LowerBound in place
//   (FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND), at the entry GetTC<CTCVillageMember> calls it.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MINIONWANDER
#define FABLE_RETAIL_CAISTATEGROUP_TAVERNSERVER
#define FABLE_RETAIL_CAISTATEGROUP_TAVERNSERVER_METHODS
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/C3DVector.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CAIStateGroup_TavernServer.h"
#include "engine/CAIBrain.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingObject.h"
#include "engine/CThingBuilding.h"
#include "engine/ThingAlive.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCTavernTable.h"
#include "engine/CTCTavernJug.h"

// Same-TU bodies (Ego_r.pdb members; retail keeps out-of-line copies of the first two and expands all four into Update).
EAIActionReturn CAIStateGroup_TavernServer::WanderWithinTavern()
{
    return AIAction_MinionWander(AI_MOVEMENT_WALKING, PPub.Get());
}

void CAIStateGroup_TavernServer::FreeServingPoint()
{
    if (ServingActionPointIndex != -1) {
        CTCTavernTable* table;
        PTable->GetTC(&table);
        table->FreeServingPoint(ServingActionPointIndex);
        ServingActionPointIndex = -1;
    }
}

inline bool CAIStateGroup_TavernServer::WalkToBar()
{
    return WalkToPoint(BarPos, BarRHSet);
}

inline bool CAIStateGroup_TavernServer::WalkToServingPoint()
{
    return WalkToPoint(ServingPos, ServingRHSet);
}

void CAIStateGroup_TavernServer::Update()
{
    CTCVillageMember* village_member;
    GetCreature().GetTC(&village_member);
    village_member->GetPVillage();
    for (;;) {
        if (ServingActionPointIndex == -1 && !GetServingPoint())
            break;
        if (!IsCarryingJug()) {
            if (!GetJug())
                break;
            if (!WalkToBar())
                break;
            if (!PickUpJug())
                break;
        }
        CTCTavernJug* jug;
        if (!IS_THING_ALIVE(PJug) || !PJug->GetTC(&jug))
            break;
        while (jug->Fullness > 0) {
            if (ServingActionPointIndex == -1 && !GetServingPoint())
                break;
            if (IsStopping())
                return;
            AIAction_DoNothing();
            if (!WalkToServingPoint())
                FreeServingPoint();
            else if (!FillTankard())
                FreeServingPoint();
        }
        while (!GetOldJugPoint()) {
            WanderWithinTavern();
            if (IsStopping())
                return;
        }
        while (!WalkToBar()) {
            WanderWithinTavern();
            if (IsStopping())
                return;
        }
        PutDownJug();
    }
    Terminate();
}
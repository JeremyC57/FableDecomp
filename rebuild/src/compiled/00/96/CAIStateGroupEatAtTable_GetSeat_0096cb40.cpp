// CAIStateGroup_EatAtTable::GetSeat() -- pick a place to eat: when the group's building (virtual GetBuilding, slot 14) is alive and
//   owns things, take the village's eating duration plus up to 45 random seconds (creature is a village member), then walk the
//   building's owned-object list and, for the first live, non-limbo thing with a tavern-table component, claim an eating point or
//   else a free seat at it (filling SeatPos / SeatRHSet / SeatActionPointIndex / SeatIndex) and remember that thing as Table.
// Leaf: ?GetSeat@CAIStateGroup_EatAtTable@@IAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0096CB40, 553 bytes, thiscall, plain `ret`, no frame pointer, no SEH, default flags.
//   Identity: Ego_r.pdb / FableWin.pdb ?GetSeat@CAIStateGroup_EatAtTable@@IAE_NXZ (protected, non-virtual); callers Update 0x0096CD70
//   and IsRunnable 0x0096D450.
// Callees: GetBuilding (vtable +0x38), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (GetTC) / const 0x004365B0
//   (PeekTC), CTCVillageMember::GetPVillage 0x007EE540, CAIStateGroupBase::GetPWorldSeed 0x008FCFD0, GFFloatRandom 0x004978A0,
//   CBaseIntelligentPointer::GetPItem 0x00A01B50, CTCTavernTable::GetEatingPoint 0x007B8B10 / GetSeat 0x007B8840,
//   CIntelligentPointer<CThing>::CIntelligentPointer(CThing*) 0x004A96F0 / operator= 0x004A9760, ~CBaseIntelligentPointer 0x00A01C10.
// Shape: the building test is IS_THING_ALIVE (inline template), which places the pre-push `return false` block between the
//   empty-list exit and the loop body (the hand-spelled `!b || b->IsDead()` puts it at the function end); GetTC/PeekTC expanded with
//   a direct bit test (no materialised presence byte); the list's end() is re-read through GetObjectList() every iteration.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_OUTOFLINE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCTHINGOWNER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCTAVERNTABLE_SEATS
#define FABLE_RETAIL_CTCTAVERNTABLE_LAYOUT
#define FABLE_RETAIL_CTCTAVERNTABLE_GETSEAT
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CVillageDef.h"
#include "engine/CTCVillage.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCThingOwner.h"
#include "engine/CTCTavernTable.h"
#include "engine/ThingAlive.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/GFFloatRandom.h"

class CAIStateGroup_EatAtTable : public CAIStateGroupBase {
protected:
    virtual void Update();                                               // slot 12
    virtual CThingBuilding* GetBuilding() const = 0;                     // slot 14
    bool GetSeat();                                                      // 0x0096CB40
    CIntelligentPointer<CThing> Table;                                   // +0x14
    long SeatIndex;                                                      // +0x1c
    long SeatActionPointIndex;                                           // +0x20
    C3DVector SeatPos;                                                   // +0x24
    CRightHandedSet SeatRHSet;                                           // +0x30
    float Duration;                                                      // +0x48
};

bool CAIStateGroup_EatAtTable::GetSeat()
{
    CThingBuilding* building = GetBuilding();
    if (!IS_THING_ALIVE(building))
        return false;
    CTCThingOwner* owner;
    if (!building->GetTC(&owner))
        return false;
    const CTCVillageMember* member;
    if (GetCreature().PeekTC(&member) && member->GetPVillage()) {
        Duration = (float)member->GetPVillage()->GetPDef()->EatDuration;
        Duration += GFFloatRandom(45.0f, GetPWorldSeed());
    }
    for (std::list<CIntelligentPointer<CThing> >::iterator it = owner->GetObjectList().begin(); it != owner->GetObjectList().end(); ++it) {
        if (IS_THING_ALIVE(*it) && !(*it)->IsInLimbo()) {
            CTCTavernTable* table;
            if ((*it)->GetTC(&table)) {
                if (table->GetEatingPoint(GetCreature(), SeatPos, SeatRHSet, SeatActionPointIndex, SeatIndex)) {
                    Table = (*it).Get();
                    return true;
                }
                if (table->GetSeat(GetCreature(), SeatPos, SeatRHSet, SeatActionPointIndex, SeatIndex, 0)) {
                    Table = (*it).Get();
                    return true;
                }
            }
        }
    }
    return false;
}
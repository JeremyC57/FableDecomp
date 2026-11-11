// CAIStateGroup_TavernServer::GetServingPoint() -- gather the live tavern tables the pub owns, then try them in random order until one hands out a serving point (position, facing, action point, seat) and a tankard; remember the table and the tankard
// Leaf: ?GetServingPoint@CAIStateGroup_TavernServer@@QAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00966DA0, 761 bytes (768 with padding); thiscall, returns bool.
// Identity: Ego_r.pdb ?GetServingPoint@CAIStateGroup_TavernServer@@QAE_NXZ (0x0098B680), between IsCarryingJug (0x0098B5B0 = retail
//   0x00966CF0) and FreeServingPoint (0x0098BA10 = retail 0x009670A0) as in retail; called by IsRunnable 0x00967650 and twice by
//   Update 0x00967D40, each time when ServingActionPointIndex is -1.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / ctor 0x00A01B10 / dtor 0x00A01C10 (temporary
//   CIntelligentPointer<CThingObject>, vtable 0x0125A858), CVectorMap::LowerBound 0x0040F020 (GetTC<CTCThingOwner>, interface 0x21),
//   malloc 0x00BFEA0E / free 0x00BFEA14 (STLport list nodes), CAIStateGroupBase::GetPWorldSeed 0x008FCFD0, GFROR13 0x00497890
//   (the inline GFRandom), CTCTavernTable::GetServingPoint 0x007B8CC0.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUP_TAVERNSERVER
#define FABLE_RETAIL_CAISTATEGROUP_TAVERNSERVER_METHODS
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCTHINGOWNER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_STLP_LIST
#include "engine/C3DVector.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CAIStateGroup_TavernServer.h"
#include "engine/CAIBrain.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingObject.h"
#include "engine/CThingBuilding.h"
#include "engine/ThingAlive.h"
#include "engine/CTCThingOwner.h"
#include "engine/CTCTavernTable.h"
#include "engine/GFRandom.h"
#include "stlport/fable_containers.h"

bool CAIStateGroup_TavernServer::GetServingPoint()
{
    CTCThingOwner* owner;
    PPub->GetTC(&owner);
    std::list<CThingObject*> tables;
    for (std::list<CIntelligentPointer<CThing> >::iterator it = owner->GetObjectList().begin(); it != owner->GetObjectList().end(); ++it)
        if (IS_THING_ALIVE(*it) && (*it)->HasTC(TCI_TAVERN_TABLE))
            tables.push_back((CThingObject*)(*it).Get());
    if (tables.empty())
        return false;
    PTable = 0;
    while (!tables.empty()) {
        long index = GFRandom(tables.size(), GetPWorldSeed());
        std::list<CThingObject*>::iterator table_it;
        for (table_it = tables.begin(); table_it != tables.end(); ++table_it) {
            if (index == 0) {
                PTable = *table_it;
                break;
            }
            --index;
        }
        CTCTavernTable* table;
        CThingObject* tankard;
        if (PTable->GetTC(&table)
            && table->GetServingPoint(GetCreature(), ServingPos, ServingRHSet, ServingActionPointIndex, SeatIndex, &tankard,
                                      CTCTavernTable::SERVING_MODE_0, true)) {
            PTankard = tankard;
            return true;
        }
        tables.erase(table_it);
    }
    return false;
}
// CAIStateGroup_GoToPub::IsRunnable() -- runnable when the creature can get a seat and a bar point: a scripted creature (AI scratchpad table and seat names both set) takes the named seat at the named table of the pub that owns it; otherwise, once NextFramePub has passed and it is tavern time in the creature's village, the nearest pub (building property 0x20) is found and, when alive and staffed, a random seat at one of its live tavern tables is tried together with the bar, then released again
// Leaf: ?IsRunnable@CAIStateGroup_GoToPub@@MAE_NXZ
// Retail: Fable.exe (TLC, VC7.1 RTM) @ 0x00949AB0, 1572 bytes, plain `ret` (protected virtual, slot 11 of vtable 0x0128BEC4;
//   `and esp,-8` frame from the 8-byte pointer temporaries). Ego_r.pdb / FableWin.pdb ?IsRunnable@CAIStateGroup_GoToPub@@MAE_NXZ.
//   Extent checked: `push ebp` entry (the previous function ends with int3 padding), last `ret` at 0x0094A0D3, next function 0x0094A0E0.
// Callees (each checked against retail with rel.py): CVectorMap::LowerBound 0x0040F020 / const 0x004365B0 (expanded GetTC / PeekTC:
//   0xAA CTCAIScratchpad, 0x33 CTCOwnedEntity, 0x23 CTCVillageMember, 0x21 CTCThingOwner; 0xB8 CTCTavernTable expanded in the loop),
//   CCharString copy ctor 0x0099EC30 / operator!=(const char*) 0x0099E960 / dtor 0x0099EAE0, CAIStateGroup_GoToPub::GetObjectWithScriptName
//   0x00947EB0 / IsStaffed 0x00947FB0 / GetSeat 0x009482A0 / FreeSeat 0x009486D0 / GetBar 0x009490D0, CBaseIntelligentPointer ctor 0x00A01B10 /
//   GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10 (vtables CIntelligentPointer<CThingObject> 0x0125A858, <CThingBuilding>
//   0x01244344, <CThing> 0x01238C6C), CIntelligentPointer<CThingBuilding>(CThingBuilding*) 0x0050F380 / operator= 0x0050F3D0 and
//   IS_THING_ALIVE<CIntelligentPointer<CThingBuilding> > 0x0065F8CA (COMDATs called out of line at the cold site), CWorld::GetFrame 0x0049D870,
//   CTCVillageMember::GetPVillage 0x007EE540, CTCVillage::IsTavernTime 0x00658C0A / GetNearestBuildingWithProperty 0x00658E38, CThing::GetPos
//   0x004C73D0, malloc 0x00BFEA0E / free 0x00BFEA14 (list nodes), list<CThingObject*> dtor 0x00792240, GetPWorldSeed 0x008FCFD0, GFROR13
//   0x00497890, CTCTavernTable::GetSeat 0x007B8840 / GetTankard 0x007B8590. Data: "" 0x0122D70E.
// Shape: the random-table half (0x00949D7C..0x0094A0B7) is CAIStateGroup_GoToPub::GetSeat 0x009482A0 itself, expanded with both
//   arguments NULL (its no-table branch 0x009483A9..0x009486CD, the table / seat tests folded away): GetSeat's body is defined in this TU
//   (retail compiles it first) with NULL default arguments, and VC7.1 expands the constant-argument site while calling the scripted
//   GetSeat(PTable, &seat_name) site. The same body compiled here is byte-identical to retail 0x009482A0 (relocation-masked, every call
//   target checked) -- that parked row can land from this TU with leaf ?GetSeat@CAIStateGroup_GoToPub@@IAE_NPAVCThing@@PBVCCharString@@@Z.
//   The scratchpad out pointer is NOT initialised (the result of GetTC is ignored): VC7.1 then homes it in a frame word it zeroes at
//   entry and reloads on the miss path (0x00949ABF / 0x00949B06), with the creature in esi (`add esi,44h`); `= 0` folds to `xor esi,esi`
//   and moves the creature to eax. The scripted exits each destroy seat_name (`if (GetSeat(..) && GetBar()) return true; ... return false;`),
//   and the pub branch frees the seat on both outcomes (two FreeSeat calls, 0x0094A0A9 / 0x00949EE3).
// Layout: shared headers plus the chunk 2020 overlay (hdr/README.md): the new engine/CAIStateGroup_GoToPub.h (Ego_r layout, PDB
//   methods) and opt-ins CTCTavernTable SEAT, CTCAIScratchpad TABLESEAT, CTCVillage TAVERN.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_GOTOPUB
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
#define FABLE_RETAIL_CTCTAVERNTABLE_SEAT
#define FABLE_RETAIL_CTCAISCRATCHPAD_TABLESEAT
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_TAVERN
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_STLP_LIST
#include "engine/CTCTavernTable.h"
#include "engine/CTCAIScratchpad.h"
#include "engine/CTCVillage.h"
#include "engine/C3DVector.h"
#include "engine/CAIStateGroup_GoToPub.h"
#include "engine/CAIBrain.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingObject.h"
#include "engine/CThingBuilding.h"
#include "engine/ThingAlive.h"
#include "engine/CTCThingOwner.h"
#include "engine/CTCOwnedEntity.h"
#include "engine/CTCVillageMember.h"
#include "engine/CWorld.h"
#include "engine/GFRandom.h"
#include "stlport/fable_containers.h"

bool CAIStateGroup_GoToPub::GetSeat(CThing* table, const CCharString* seat_name)
{
    if (table && seat_name) {
        PTable = (CThingObject*)table;
        CTCTavernTable* tavern_table;
        if (PTable->GetTC(&tavern_table)
            && tavern_table->GetSeat(GetCreature(), Pos, RHSet, ActionPointIndex, SeatIndex, seat_name)) {
            PTankard = tavern_table->GetTankard(SeatIndex);
            return true;
        }
        ActionPointIndex = -1;
        return false;
    }
    std::list<CThingObject*> tables;
    CTCThingOwner* owner;
    if (!IS_THING_ALIVE(PPub) || !PPub->GetTC(&owner))
        return false;
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
        CTCTavernTable* tavern_table;
        if (PTable->GetTC(&tavern_table)
            && tavern_table->GetSeat(GetCreature(), Pos, RHSet, ActionPointIndex, SeatIndex, 0)) {
            PTankard = tavern_table->GetTankard(SeatIndex);
            return true;
        }
        tables.erase(table_it);
    }
    ActionPointIndex = -1;
    return false;
}

bool CAIStateGroup_GoToPub::IsRunnable()
{
    CTCAIScratchpad* scratchpad;
    GetCreature().GetTC(&scratchpad);
    UnderScriptControl = scratchpad->GetSeatName() != "" && scratchpad->GetTableName() != "";
    if (UnderScriptControl) {
        PTable = GetObjectWithScriptName(scratchpad->GetTableName());
        CTCOwnedEntity* owned = 0;
        PTable->GetTC(&owned);
        CCharString seat_name = scratchpad->GetSeatName();
        if (owned) {
            PPub = (CThingBuilding*)owned->GetPOwner();
            if (GetSeat(PTable.Get(), &seat_name) && GetBar())
                return true;
        }
        return false;
    }
    if (NextFramePub < CWorld::GetFrame()) {
        const CTCVillageMember* member;
        if (GetCreature().PeekTC(&member)) {
            CTCVillage* village = member->GetPVillage();
            if (village && village->IsTavernTime()) {
                PPub = village->GetNearestBuildingWithProperty(GetCreature().GetPos(), (ESimBuildingProperty)0x20);
                if (IS_THING_ALIVE(PPub)) {
                    if (IsStaffed() && GetSeat() && GetBar()) {
                        FreeSeat();
                        return true;
                    }
                    FreeSeat();
                    return false;
                }
            }
        }
    }
    return false;
}

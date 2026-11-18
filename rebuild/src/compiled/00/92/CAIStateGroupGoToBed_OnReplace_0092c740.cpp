// CAIStateGroup_GoToBed::OnReplace() -- give the bed's action point (or the bed itself) back and forget the bed
// Leaf: ?OnReplace@CAIStateGroup_GoToBed@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092C740, length 347 bytes (vtable 0x0128B104 slot 10).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020
//          (the inlined GetInterface: type 0x2D = bit 13 of the bitset dword +0x24, type 0xAC = bit 12 of +0x34),
//          the action-point owner's FreePoint(long) @ 0x007FEF40 (landed as CPointerListOwner_007fef40::Add: appends the
//          index to its free list), CTCSinglePersonOccupiable::GetOccupier @ 0x0087C3E0 and SetOccupier(CThingCreatureBase*) @ 0x0087C390
//          (manifest labels "_Cons_val<...>" / "CVillageTaskCarryCrate::AssignCreatureToTask" are propagated),
//          CBaseIntelligentPointer ctor @ 0x00A01B10, SetPItem @ 0x00A01B90, dtor @ 0x00A01C10 (the PBed = 0 temporary,
//          vtable 0x01238C6C).
// Shape: IS_THING_ALIVE is the two-GetPItem inline test, the component comes from a third GetPItem through the
//          out-parameter GetInterface (miss path reads the never-written out slot, which the clearing temporary
//          later reuses); the -1 marker stays in edi for the store; the occupant is compared with the creature
//          read before the call.
// Layout: PDB CAIStateGroup_GoToBed: PBed +0x14, IsBedSleepPointMarker +0x1C, ActionPointIndex +0x2C (retail agrees,
//          see the landed GoToBed rows). engine/CAIStateGroup_GoToBed.h (retail branch).
// NF79 (round 7, q10956): ported onto the shared headers (engine/CAIStateGroup_GoToBed.h retail branch, the macro block of the landed
//   IsRunnable 0x0092D6F0, CTCUserActionPointBase.h, CTCSinglePersonOccupiable.h): the interface-172 component (key 0xAC) is
//   CTCSinglePersonOccupiable (Ego_r.pdb / FableWin.pdb GetOccupier / SetOccupier, donor order = retail 0x0087C3E0 / 0x0087C390), not a
//   CTCBed (CTCBed is interface 53 and has no such member in either PDB); every local model is gone.
#define FABLE_RETAIL_CAISTATEGROUP_GOTOBED
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCUSERACTIONPOINTBASE
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/ThingAlive.h"
#include "engine/CAIStateGroup_GoToBed.h"
#include "engine/CTCUserActionPointBase.h"
#include "engine/CTCSinglePersonOccupiable.h"

void CAIStateGroup_GoToBed::OnReplace()
{
    if (!IsBedSleepPointMarker) {
        if (ActionPointIndex != -1 && IS_THING_ALIVE(PBed)) {
            CTCUserActionPointBase* ptc_points;
            PBed->GetTC(TCI_SINGLE_USER_ACTION_POINTS, &ptc_points);
            ptc_points->SetActionPointAsFree(ActionPointIndex);
            ActionPointIndex = -1;
        }
    }
    else if (IS_THING_ALIVE(PBed)) {
        CTCSinglePersonOccupiable* ptc_bed;
        PBed->GetTC(TCI_SINGLE_PERSON_OCCUPIABLE, &ptc_bed);
        CThingAICreature* creature = &GetCreature();
        if (ptc_bed->GetOccupier() == creature)
            ptc_bed->SetOccupier(0);
    }
    PBed = CIntelligentPointer<CThing>(0);
}

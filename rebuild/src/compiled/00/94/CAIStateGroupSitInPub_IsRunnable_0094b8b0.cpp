// CAIStateGroup_SitInPub::IsRunnable() -- for a villager: find the nearest pub building of its village (property
// 0x20) from the creature's position, join it as a customer, remember it and pick a seat (a bench with one person
// sitting there, else a random free one); runnable while the seat is valid.
// Leaf: ?IsRunnable@CAIStateGroup_SitInPub@@MAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094B8B0, length 245 bytes (+ int3 padding). Vtable slot 11 of
// CAIStateGroup_SitInPub (0x0128BF04).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inline GetTC: TCI_VILLAGE_MEMBER 0x23 bit 3 of +0x24; TCI_GATHER_POINT 0x7B bit 27 of
// +0x2c, HasTC first), CTCVillageMember::GetPVillage @ 0x007EE540, CThing::GetPos @ 0x004C73D0,
// CTCVillage::GetNearestBuildingWithProperty @ 0x00658E38, CTCGatherPointBase::AddAsUser @ 0x007FFEB0, the out-of-line
// CIntelligentPointer<CThing>(CThing*) @ 0x004A96F0 / operator= @ 0x004A9760 / ~CBaseIntelligentPointer @ 0x00A01C10,
// GetBenchWithOnePersonSittingThere @ 0x0094B840, CAIStateGroup_UsesSeat::GetRandomFreeSeatInBuilding @ 0x00938BF0 / SetSeat @ 0x009106E0 /
// IsSeatValid @ 0x00938580.
// Shape: `PBuilding = pub;` -- the CIntelligentPointer<CThing> temporary is an IMPLICIT conversion through the out-of-line
// constructor, so the out-of-line operator= gets `lea eax,[temp]`; the seat is `if (bench) SetSeat(bench); else SetSeat(random);`
// (the two calls cross-jump, `this` set in both predecessors).
// NF67 (round 7): ported onto the shared headers. The local CThing / CThingPos / CBaseIntelligentPointer / CIntelligentPointerConstThing /
//   CIntelligentPointerThingB / CAIBrain / CAIStateGroupBase / CTCVillage / CTCVillageMember / CTCGatherPointBase / CAIStateGroup_UsesSeat
//   models are gone; the class chain comes from overlay opt-ins FABLE_RETAIL_CAISTATEGROUP_SITINPUB (-> _USESSEAT, new overlay header
//   engine/CAIStateGroup_UsesSeat.h). 0x00658E38 now binds the PDB GetNearestBuildingWithProperty (nf41:e93); 0x004A96F0 / 0x004A9760 bind
//   ??0?$CIntelligentPointer@VCThing@@@@QAE@PAVCThing@@@Z / ??4?$CIntelligentPointer@VCThing@@@@QAEAAV0@ABV0@@Z; 0x0094B840 is the PDB
//   GetBenchWithOnePersonSittingThere (was the stand-in FindAndValidateTask).
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
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_OUTOFLINE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_TAVERN
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAISTATEGROUP_SITINPUB
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCVillage.h"
#include "engine/CTCGatherPointBase.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_SitInPub.h"

bool CAIStateGroup_SitInPub::IsRunnable()
{
    CTCVillageMember* villager;
    if (GetCreature().GetTC(&villager)) {
        CTCVillage* village = villager->GetPVillage();
        if (village) {
            CThingBuilding* pub = village->GetNearestBuildingWithProperty(GetCreature().GetPos(), (ESimBuildingProperty)0x20);
            if (pub && pub->HasTC(TCI_GATHER_POINT)) {
                CTCGatherPointBase* gather;
                pub->GetTC(TCI_GATHER_POINT, &gather);
                if (gather->AddAsUser(GetCreature())) {
                    PBuilding = pub;
                    CThingObject* seat = GetBenchWithOnePersonSittingThere(*pub);
                    if (seat)
                        SetSeat(seat);
                    else
                        SetSeat(GetRandomFreeSeatInBuilding(*pub));
                }
            }
        }
    }
    return IsSeatValid();
}

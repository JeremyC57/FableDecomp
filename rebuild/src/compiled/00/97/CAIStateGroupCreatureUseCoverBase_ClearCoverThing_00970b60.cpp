// CAIStateGroup_CreatureUseCoverBase::ClearCoverThing() -- if the cover thing is still alive, release its cover-point component (SetUser(0)) and clear PCoverThing
// Leaf: ?ClearCoverThing@CAIStateGroup_CreatureUseCoverBase@@IAEXXZ
// NF72 (round 7, q10886): access-only fix: ClearCoverThing is protected in Ego_r.pdb (IAE; engine/CAIStateGroup_CreatureUseCoverBase.h TAKECOVER,
//   bound so by the ported 0x0097A630); the local declaration is now under `protected:`. The bytes are unchanged.
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00970B60, length 166 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three reads: IS_THING_ALIVE inlined, then the thing),
// CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inline out-parameter GetInterface, type 0xAC),
// CTCSinglePersonOccupiable::SetOccupier(0) @ 0x0087C390 (Ego_r.pdb / FableWin.pdb; was the stand-in CTCCoverPoint::SetUser),
// CBaseIntelligentPointer ctor/SetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01C10.
// Shape: the reset `PCoverThing = CIntelligentPointer<CThing>(0)` (vtable 0x01238C6C) runs only inside the alive test;
// the out-parameter lookup keeps the component in eax (`mov eax,[eax+4] ; push 0 ; mov ecx,eax`).
// Layout: Ego_r.pdb CAIStateGroup_CreatureUseCoverBase (PCoverThing +0x1f, CoverList +0x28, TimingCritical +0x1e).
// NF79 (round 7, q10956): ported onto the shared headers with the macro block of NF76's sibling 0x00970CD0 (same class): the interface-172
//   component is CTCSinglePersonOccupiable (engine/CTCSinglePersonOccupiable.h; GetTC key 0xAC = TCI_SINGLE_PERSON_OCCUPIABLE, as the old
//   comment records) and 0x0087C390 its SetOccupier; the local CBaseIntelligentPointer / CIntelligentPointer / CAIBrain / CTCCoverPoint /
//   CCoverList / CAIStateGroup_CreatureUseCoverBase models are gone.
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
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_SUSPENDABLE_BASE
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VSLOTS
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_COVERLIST
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_ADVANCE
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_TYPED_COVERLIST
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_TAKECOVER
#include "rebuild_abi.h"
#include "fable_compiler.h"
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CThingAICreature.h"
#include "engine/CWorld.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_CreatureUseCoverBase.h"
#include "engine/CTCSinglePersonOccupiable.h"

void CAIStateGroup_CreatureUseCoverBase::ClearCoverThing()
{
    if (IS_THING_ALIVE(PCoverThing)) {
        CTCSinglePersonOccupiable* ptc_cover;
        if (PCoverThing->GetTC(TCI_SINGLE_PERSON_OCCUPIABLE, &ptc_cover))
            ptc_cover->SetOccupier(0);
        PCoverThing = CIntelligentPointer<CThing>(0);
    }
}

// CAIStateGroup_CreatureUseCoverBase::CleanupLeaf() -- release a live cover thing's cover point (SetOccupier(0)), empty the cover list, clear PCoverThing and TimingCritical
// Leaf: ?CleanupLeaf@CAIStateGroup_CreatureUseCoverBase@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00970C10, length 192 bytes.
// Identity: Ego_r.pdb (0x00995F40) + FableWin.pdb (0x02B85A70) ?CleanupLeaf@CAIStateGroup_CreatureUseCoverBase@@MAEXXZ: the protected override of
//   CAIStateGroup_EnemyCreatureBase's virtual CleanupLeaf (the landed leaf was the public non-virtual QAE spelling).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (type 0xAC),
//   CTCSinglePersonOccupiable::SetOccupier(0) @ 0x0087C390 (Ego_r.pdb / FableWin.pdb; was the stand-in CTCCoverPoint::SetUser),
//   vector<CIntelligentPointer<CThing> >::erase(first, last) @ 0x004AC0F0 (clear(), the erase out of line), CBaseIntelligentPointer ctor / SetPItem /
//   dtor @ 0x00A01B10 / 0x00A01B90 / 0x00A01C10.
// Shape: the component pointer is deliberately unset when the interface is absent (retail calls through the dead key slot), so the GetTC result
//   is ignored before the SetOccupier(0) call.
// NF79 (round 7, q10956): ported onto the shared headers with the 0x00970B60 / NF76 0x00970CD0 macro block plus my overlay opt-in
//   CAISTATEGROUP_CREATUREUSECOVERBASE_CLEANUPLEAF and the repo opt-in STLP_VECTOR_ERASE_RANGE_OUTOFLINE; every local model is gone.
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
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_CLEANUPLEAF
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
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


void CAIStateGroup_CreatureUseCoverBase::CleanupLeaf()
{
    if (IS_THING_ALIVE(PCoverThing)) {
        CTCSinglePersonOccupiable* ptc_cover;
        PCoverThing->GetTC(TCI_SINGLE_PERSON_OCCUPIABLE, &ptc_cover);
        ptc_cover->SetOccupier(0);
    }
    CoverList.clear();
    PCoverThing = CIntelligentPointer<CThing>(0);
    TimingCritical = false;
}

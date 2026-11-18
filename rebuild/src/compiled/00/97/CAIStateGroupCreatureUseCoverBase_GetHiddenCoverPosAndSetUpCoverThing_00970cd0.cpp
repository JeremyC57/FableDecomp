// CAIStateGroup_CreatureUseCoverBase::GetHiddenCoverPosAndSetUpCoverThing(C3DVector&) -- find a live available cover, release old cover, retain and occupy the new one; yield after a failed position test.
// Retail 0x00970CD0, 277 bytes.
// Leaf: ?GetHiddenCoverPosAndSetUpCoverThing@CAIStateGroup_CreatureUseCoverBase@@IAE_NAAVC3DVector@@@Z
// NF76 port (2026-11-17): rewritten on the retail CAIStateGroup_CreatureUseCoverBase class (the 0x0097CCD0 / 0x00970590 macro block:
//   CREATUREUSECOVERBASE + ADVANCE + TAKECOVER + TYPED_COVERLIST over the EnemyCreatureBase / CAIStateGroupBase / CAIBrain retail
//   branches); every local model deleted (UseCoverMethods, CThingRetail, CIntelligentPointerRetail, CIntelligentVector, CBrainDispatch,
//   CTCOccupiableMethods, CVectorMap / pair / ETCInterfaceType). Bindings now under their PDB spellings: ClearCoverThing 0x00970B60
//   ?ClearCoverThing@CAIStateGroup_CreatureUseCoverBase@@IAEXXZ (queue q10886), GetHiddenCoverPos 0x00970930 (Ego_r.pdb, ABVCThing),
//   the interface-172 component CTCSinglePersonOccupiable (engine header; overlay ISOCCUPIED: IsOccupied 0x0087C3F0, Ego_r.pdb) with
//   SetOccupier 0x0087C390 (Ego_r.pdb), and the brain's SuspendSuspendableProcess (CSuspendableProcessMicrothread slot 2, 0x00A44840,
//   CAIBRAIN_SUSPENDABLE_BASE). Self: Ego_r.pdb ?GetHiddenCoverPosAndSetUpCoverThing@..@@IAE_NAAVC3DVector@@@Z (protected; the landed
//   leaf was the stand-in UseCoverMethods' QAE). Locals as Ego_d.pdb names them (cover_pos, it, pthingobj, ptc_new_cover).
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
#define FABLE_RETAIL_CTCSINGLEPERSONOCCUPIABLE_ISOCCUPIED
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

bool CAIStateGroup_CreatureUseCoverBase::GetHiddenCoverPosAndSetUpCoverThing(C3DVector& cover_pos)
{
    if (CoverList.empty())
        return false;
    std::vector<CIntelligentPointer<CThing> >::iterator it = CoverList.begin();
    do {
        if (!IS_THING_ALIVE(PTargetThing))
            return false;
        CIntelligentPointer<CThing>& pthingobj = *it++;
        if (IS_THING_ALIVE(pthingobj)) {
            if (GetHiddenCoverPos(cover_pos, *pthingobj.Get())) {
                CTCSinglePersonOccupiable* ptc_new_cover;
                if (pthingobj->GetTC(&ptc_new_cover) && !ptc_new_cover->IsOccupied()) {
                    ClearCoverThing();
                    PCoverThing = pthingobj;
                    ptc_new_cover->SetOccupier(PBrain->Creature);
                    return true;
                }
            } else
                PBrain->SuspendSuspendableProcess();
        }
    } while (it != CoverList.end());
    return false;
}

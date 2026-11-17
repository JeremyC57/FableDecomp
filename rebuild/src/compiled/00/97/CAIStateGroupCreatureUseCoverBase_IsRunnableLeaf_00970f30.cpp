// CAIStateGroup_CreatureUseCoverBase::IsRunnableLeaf() -- after a failed cover search wait 8 seconds or 15 units of movement; then gather the cover things: none records the failure here, otherwise filter them into the retained cover handles
// Leaf: ?IsRunnableLeaf@CAIStateGroup_CreatureUseCoverBase@@MAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00970F30, 402 bytes.
// Identity (NF56, round 7, queue q10460): Ego_r.pdb 0x00995CE0 / FableWin.pdb 0x02B85370 ?IsRunnableLeaf@CAIStateGroup_CreatureUseCoverBase@@MAE_NXZ
//   (protected virtual: the override of CAIStateGroup_EnemyCreatureBase slot 15, retail vtable 0x0128D31C slot 15 = 0x00970F30). Was the
//   stand-in UseCoverMethods::IsRunnableLeaf (a local struct over local CAIStateGroupBase / CWorld / CMainGameComponent / CThingRetail /
//   CAIBrainRetail / CCoverDispatch models). Now on the retail class of engine/CAIStateGroup_CreatureUseCoverBase.h (opt-in _COVERLIST:
//   slots 23 GetCoverThingList / 22 ProcessCoverThingList on the PDB vector<CThing*>&), with GetCreature() (PBrain +4, Creature +0x18),
//   CThing::GetPos 0x004C73D0, CAIStateGroupBase::GetWorld 0x008FCFC0, CWorld::GetFrame 0x0049D870 and the ConstantFPS read
//   (0x01375550) through CWorld::GetFramesPerSecond (opt-in FABLE_RETAIL_CWORLD_GETFRAMESPERSECOND, descriptive inline: retail calls
//   GetWorld and discards it). CoverList is the PDB vector<CIntelligentPointer<CThing> > (push_back overflow 0x004AC940).
// Shape: C3DVector offset (operator-, Z dead) keeps retail frame slots; positive !empty() block puts the failure path last.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SQUARED2D_ONE_EXPRESSION_PASS23
#define FABLE_RETAIL_STLP_MALLOC_ALLOC_DEALLOCATE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_GETFRAMESPERSECOND
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VSLOTS
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_COVERLIST
#include "fable_compiler.h"
#include "rebuild_abi.h"
#include <stddef.h>
#include <stdlib.h>
#include "stlport/fable_containers.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingAICreature.h"
#include "engine/CWorld.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_CreatureUseCoverBase.h"

bool CAIStateGroup_CreatureUseCoverBase::IsRunnableLeaf()
{
    if (FailedToFindCover) {
        C3DVector offset = FailedToFindCoverAt - GetCreature().GetPos();
        long framesPerSecond = GetWorld().GetFramesPerSecond();
        long seconds = (CWorld::GetFrame() - FailedTime) / framesPerSecond;
        if (offset.GetSquaredMagnitude2D() < 225.0f && seconds < 8)
            return false;
        FailedToFindCover = false;
    }
    std::vector<CThing*> things;
    GetCoverThingList(things);
    if (!things.empty()) {
        ProcessCoverThingList(things);
        for (std::vector<CThing*>::iterator it = things.begin(); it != things.end(); ++it)
            (*(std::vector<CIntelligentPointer<CThing> >*)CoverList).push_back(CIntelligentPointer<CThing>(*it));
        return true;
    }
    FailedToFindCoverAt = GetCreature().GetPos();
    FailedToFindCover = true;
    return false;
}

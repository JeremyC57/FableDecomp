// CAIStateGroup_CreatureAdvanceInCover::Update() -- advance to the next hidden cover position, crouch there when the enemy is alive, then refresh the cover list
// Leaf: ?Update@CAIStateGroup_CreatureAdvanceInCover@@UAEXXZ
// Port (NF68): on the landed MoveIntoCover 0x009711D0 macro block (UseCoverBase / EnemyCreatureBase / CAIStateGroupBase retail
//   branches, CIntelligentPointer, IS_THING_ALIVE) + RefreshCoverList 0x009710D0 (COVERLIST), with overlay opt-ins
//   FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_ADVANCE / _TYPED_COVERLIST, FABLE_RETAIL_CAISTATEGROUP_CREATUREADVANCEINCOVER_UPDATE,
//   FABLE_RETAIL_CCREATUREACTION_PROJECTILECROUCH_CLASS. The local CAIStateGroupBase / EnemyCreatureBase / UseCoverBase / CCombatActionBase /
//   CCreatureAction_ProjectileCrouch / CAIBrain / vector models are gone.
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
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VSLOTS
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_COVERLIST
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_ADVANCE
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_TYPED_COVERLIST
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREADVANCEINCOVER_ISRUNNABLELEAF
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREADVANCEINCOVER_UPDATE
#define FABLE_RETAIL_CCREATUREACTION_PROJECTILECROUCH_CLASS
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#include "rebuild_abi.h"
#include "fable_compiler.h"
#include "stlport/fable_containers.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/NCreatureMode.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CWorld.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_CreatureUseCoverBase.h"
#include "engine/CAIStateGroup_CreatureAdvanceInCover.h"
#include "engine/CCreatureAction_ProjectileCrouch.h"

void CAIStateGroup_CreatureAdvanceInCover::Update()
{
    CThing* previous_cover = 0;
    if (IS_THING_ALIVE(PCoverThing.Get()))
        previous_cover = PCoverThing.Get();
    C3DVector cover_pos;
    if (!GetHiddenCoverPosAndSetUpCoverThing(cover_pos)) {
        SetAtFailed();
        Terminate();
        return;
    }
    const C3DVector& pos = GetCreature().GetPos();
    C2DVector offset(cover_pos.X - pos.X, cover_pos.Y - pos.Y);
    float distance = offset.GetMagnitude();
    if (distance > 1.8f) {
        if (IS_THING_ALIVE(previous_cover)) {
            if (!IS_THING_ALIVE(PTargetThing)) {
                Terminate();
                return;
            }
            AIAction_PerformActionAndWait(CCreatureAction_ProjectileCrouch(GetCreature(), *PTargetThing.Get(), cover_pos), false);
        }
        if (MoveIntoCover(cover_pos))
            DuckBehindCover(true);
    }
    if (!IsStillRunnable()) {
        CoverList.clear();
        Terminate();
        return;
    }
    RefreshCoverList();
    RemoveThingFromCoverList(PCoverThing.Get());
    if (CoverList.size() == 0)
        Terminate();
}

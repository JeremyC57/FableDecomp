// CAIStateGroup_CreatureUseCoverBase::SetAtFailed() -- remember where and when the cover search failed
// Leaf: ?SetAtFailed@CAIStateGroup_CreatureUseCoverBase@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00970590, length 54 bytes (+ int3 padding).
// Callees: CThing::GetPos @ 0x004C73D0 (copied memberwise into FailedToFindCoverAt), CAIStateGroupBase::GetWorld @ 0x008FCFC0 (called for its
//   result object only), CWorld::GetFrame @ 0x0049D870 (static, __fastcall).
// NF72 (round 7, q10885): Ego_r.pdb ?SetAtFailed@CAIStateGroup_CreatureUseCoverBase@@IAEXXZ (protected), declared by
//   engine/CAIStateGroup_CreatureUseCoverBase.h ADVANCE (NF68); the ported callers 0x009790F0 / 0x0097A630 bind it. The landed self row spelled it
//   with the descriptive public SetFailedToFindCover on local CAIStateGroupBase / CAIBrain / CThingAICreature / CWorld models; ported onto the
//   shared headers with the 0x009790F0 macro block (FailedToFindCover +0x34, FailedTime +0x38, FailedToFindCoverAt +0x3C).
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
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
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

void CAIStateGroup_CreatureUseCoverBase::SetAtFailed()
{
    FailedToFindCover = true;
    FailedToFindCoverAt = GetCreature().GetPos();
    FailedTime = GetWorld().GetFrame();
}

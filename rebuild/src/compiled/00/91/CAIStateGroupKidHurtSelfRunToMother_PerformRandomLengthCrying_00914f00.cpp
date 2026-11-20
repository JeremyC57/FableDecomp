// CAIStateGroup_KidHurtSelfRunToMother::PerformRandomLengthCrying() -- play the crying-on-the-spot into animation, the loop animation 0..2 extra times (world-seed random), then the out-of animation; returns the first non-success action result
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00914F00, length 384 bytes (+ int3 padding).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 ("ST_CRYING_ON_THE_SPOT_INTO"
//          @ 0x0128FA60, "..._LOOP" @ 0x0128FA44, "..._OUTOF" @ 0x0128FA28), CThingCreatureBase::GetAnimFromName
//          @ 0x00662A00, CCreatureAction_PlayAnimation ctors @ 0x00842570 (8 args) / 0x008425E0 (11 args, looping),
//          CAIStateGroupBase::AIAction_PerformActionAndWait @ 0x0097DB70, ~CCreatureActionBase @ 0x00693EF0,
//          CAIStateGroupBase::GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890.
// Shape: each animation is a block with a named CCharString; GetCreature() is written twice (ctor argument and `this` of
//          GetAnimFromName) and VC7.1 loads it once into edi at the second use; the action temporary is built in the PerformActionAndWait call; the loop count is the
//          *seed-form random (`% 3`), kept in ebp, pushed mid-function.
// Layout: engine/CAIStateGroup_KidHurtSelfRunToMother.h (PBrain is the first dword of the base at +0x04).
// Leaf: ?PerformRandomLengthCrying@CAIStateGroup_KidHurtSelfRunToMother@@QAE?AW4EAIActionReturn@@XZ
// NF131 (2026-11-20, q5141 / q5021): the stand-in CAIStateGroupKidHurtSelfRunToMotherMethods over the flat generated header and its local
//   CCharString / CSoundPair / CThing / CCreatureActionBase / CCreatureAction_PlayAnimation / CAIBrain models give way to the retail shared
//   headers (the define / include block of the landed sibling 0x00915350). Own name: Ego_r.pdb 0x00938AE0 + FableWin.pdb 0x02B9EB60
//   ?PerformRandomLengthCrying@CAIStateGroup_KidHurtSelfRunToMother@@QAE?AW4EAIActionReturn@@XZ (the spelling its caller 0x00915350 binds), was
//   ?PerformRandomLengthCrying@CAIStateGroupKidHurtSelfRunToMotherMethods@@QAEJXZ. Callees now: ?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ 0x008FCFD0
//   and ?AIAction_PerformActionAndWait@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@ABVCCreatureActionBase@@_N@Z 0x0097DB70 (both PDBs), the
//   header CCreatureAction_PlayAnimation ctors / GetAnimFromName; the local Random step is the shared inline GFRandom.
#include "fable_compiler.h"
#define FABLE_RETAIL_KID_HURT_SELF_PASS23
#define FABLE_RETAIL_KID_MOTHER_REFERENCE_PASS23
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_SAVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#define FABLE_RETAIL_CSOUNDPAIR_ORIGINAL_CTORS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CINITBASECLASS
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_C3DVECTOR
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_KidHurtSelfRunToMother.h"
#include "engine/CCreatureAction_PlayAnimation.h"
#include "engine/CSoundPair.h"
#include "engine/CThingFilter_CanSeeEachOther.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/ThingAlive.h"
#include "engine/GFRandom.h"
#include "engine/GFWorldDistance.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"

EAIActionReturn CAIStateGroup_KidHurtSelfRunToMother::PerformRandomLengthCrying()
{
    EAIActionReturn result;
    {
        CCharString name("ST_CRYING_ON_THE_SPOT_INTO");
        result = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(),
            GetCreature().GetAnimFromName(name, 0), CSoundPair(0), false, true, false, 0, 0), false);
    }
    if (result != AI_ACTION_FINISHED)
        return result;
    long loops = GFRandom(3, GetPWorldSeed());
    {
        CCharString name("ST_CRYING_ON_THE_SPOT_LOOP");
        result = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(),
            GetCreature().GetAnimFromName(name, 0), false, true, loops, CSoundPair(0), false, true, false, 0, 0), false);
    }
    if (result != AI_ACTION_FINISHED)
        return result;
    {
        CCharString name("ST_CRYING_ON_THE_SPOT_OUTOF");
        result = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(),
            GetCreature().GetAnimFromName(name, 0), CSoundPair(0), false, true, false, 0, 0), false);
    }
    return result;
}

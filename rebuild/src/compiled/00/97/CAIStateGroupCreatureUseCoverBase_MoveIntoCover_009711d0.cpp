// CAIStateGroup_CreatureUseCoverBase::MoveIntoCover(const C3DVector& cover_pos) -- dive into cover when 2.4..2.8 units away (and not strafing), otherwise strafe there firing at the target; counts the successful arrivals
// Leaf: ?MoveIntoCover@CAIStateGroup_CreatureUseCoverBase@@IAE_NABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009711D0, 516 bytes, `ret 4` (Ego_r.pdb name above, protected).
// Callees: CThing::PeekTC<const CTCCreatureModeManager> (expanded; const CVectorMap::LowerBound 0x004365B0),
//   CTCCreatureModeManager::IsModeActive 0x0073A8A0 (MODE_STRAFE), IS_THING_ALIVE (GetPItem 0x00A01B50), CAIStateGroupBase::Terminate
//   0x008FCF40, CThing::GetPos 0x004C73D0, CCreatureAction_DuckAndCover(CThingCreatureBase&, const C3DVector&) 0x009717A0 and its
//   compiler-generated destructor (~CCombatActionBase 0x0062E900), CAIStateGroupBase::AIAction_PerformActionAndWait 0x0097DB70,
//   CAIStateGroupBase::GetWorld 0x008FCFC0, CAIGameCameraBase::PeekCurrentPos (slot 11, +0x2C), the CIntelligentPointer<const CThing>
//   conversion (0x00A01B10 / 0x00A01B50 / 0x00A01B90), CAIStateGroupBase::AIAction_ProjectileAttackAndStrafeToPos 0x0097F3A0.
// Shape: written on the shared headers (CAIStateGroup_CreatureUseCoverBase / CAIStateGroup_EnemyCreatureBase / CAIStateGroupBase
//   retail branches, GFGetWorld2DDistance, CCreatureAction_DuckAndCover); the port alone gives retail's this-in-edi colouring (the
//   earlier local model had this in esi). GFGetWorld2DDistance names the squared distance before the sqrt (0x00971332: both
//   differences popped before fsqrt).
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
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_GFGETWORLD2DDISTANCE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_PROJECTILESTRAFE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE
#include "rebuild_abi.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/NCreatureMode.h"
#include "engine/C3DVector.h"
#include "engine/GFWorldDistance.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CWorld.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_CreatureUseCoverBase.h"
#include "engine/CCreatureAction_DuckAndCover.h"

bool CAIStateGroup_CreatureUseCoverBase::MoveIntoCover(const C3DVector& cover_pos)
{
    TimingCritical = true;
    const CTCCreatureModeManager* modes;
    GetCreature().PeekTC(&modes);
    bool strafing = modes->IsModeActive(NCreatureMode::MODE_STRAFE);
    EAIActionReturn ret;
    bool nav_route_calculated;
    do {
        if (!IS_THING_ALIVE(PTargetThing)) {
            Terminate();
            TimingCritical = false;
            return false;
        }
        C3DVector offset = cover_pos - GetCreature().GetPos();
        float distance = offset.GetMagnitude2D();
        if (!strafing && HasDiveAnim && distance > 2.4f && distance < 2.8f) {
            ret = AIAction_PerformActionAndWait(CCreatureAction_DuckAndCover(GetCreature(), offset), false);
        } else {
            ret = AIAction_ProjectileAttackAndStrafeToPos(ACTION_REPEAT_CALLBACK, cover_pos, PTargetThing, nav_route_calculated,
                AI_MOVEMENT_RUNNING, GFGetWorld2DDistance(GetCreature().GetPos(), GetWorld().GetGameCamera()->PeekCurrentPos()) < 10.0f);
        }
    } while (ret == AI_ACTION_IN_PROGRESS);
    TimingCritical = false;
    if (ret == AI_ACTION_FINISHED) {
        ++ValidPositionsFound;
        return true;
    }
    return false;
}
// CAIStateGroup_JackOfBladesStage3::Update()
// Leaf: ?Update@CAIStateGroup_JackOfBladesStage3@@UAEXXZ
// Flags: (none)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CAISTATEGROUP_JACKOFBLADESBASE
#define FABLE_RETAIL_JACKOFBLADESBASE_STAGE2_UPDATE
#define FABLE_RETAIL_CTCBOSS_ISREADYFORPHASECHANGE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CJACKOFBLADESBATTLEDEF_TYPENAME_PASS26
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SCALE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION_NOWAIT
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_JACK_STAGE3_ACTIONS_PASS26
#define FABLE_RETAIL_CCOUNTEDPOINTER_ORIGINAL_COMPLETE_MEMBERS_PASS22
#define FABLE_RETAIL_CCREATUREACTION_PLAYCOMBATANIMATION
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CAIStateGroup_JackOfBladesStage3.h"
#include "engine/CAIBrain.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CDefinitionManager.h"
#include "engine/CJackOfBladesBattleDef.h"
#include "engine/CTCBoss.h"
#include "engine/CCreatureAction_JackScream.h"
#include "engine/CCreatureAction_JackFall.h"
#include "engine/CCreatureAction_JackDeathInto.h"
#include "engine/CCreatureAction_PlayCombatAnimation.h"
#include "engine/ThingAlive.h"
#include "engine/GFRandom.h"

// Original same-TU helper in FableWin 0x02BE5100 and retail 0x00971B00.
inline bool CAIStateGroup_JackOfBladesBase::IsStillRunnable()
{
    if (IS_THING_ALIVE(PTargetThing) && !IsStopping() && IS_THING_ALIVE(PArenaCentre))
        return true;
    return false;
}

void CAIStateGroup_JackOfBladesStage3::Update()
{
    CTCBoss* ptc_boss;
    GetCreature().GetTC(&ptc_boss);

    CDefPointer<const CJackOfBladesBattleDef> p_jack_def;
    GetCreature().PeekBaseDef().GetSubDef(&p_jack_def);

    if (IS_THING_ALIVE(PSwordEmitter))
        PSwordEmitter->Kill(true);

    GetCreature().GetPhysicsTC()->SetFacingAngleXY(0.0f);
    AIAction_PerformActionAndWait(CCreatureAction_JackScream(GetCreature()), false);

    long start_time = GetWorld().GetFrame();
    long timer;
    do {
        if (!IsStillRunnable()) {
            Terminate();
            return;
        }
        AIAction_DoNothing();
        timer = GetWorld().GetFrame() - start_time;
    } while (timer < p_jack_def->ScreamInAirTime * CMainGameComponent::GetConstantFPS());

    float current_scale = GetCreature().GetTCGraphicAppearance()->GetScale();
    float normal_scale = 1.2f;
    float scale_delta = (normal_scale - current_scale) /
        (CMainGameComponent::GetConstantFPS() * p_jack_def->ScaleDownTime);
    while (current_scale > normal_scale && IsStillRunnable()) {
        current_scale = GetCreature().GetTCGraphicAppearance()->GetScale();
        current_scale += scale_delta;
        GetCreature().GetTCGraphicAppearance()->SetScale(current_scale);
        AIAction_DoNothing();
    }
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }

    GetCreature().GetTCGraphicAppearance()->SetScale(normal_scale);
    GetCreature().GetPhysicsTC()->EnableGravity(true);
    AIAction_PerformActionAndWait(CCreatureAction_JackFall(GetCreature()), false);
    ptc_boss->SetNextBossPhase(5, false);

    if (!IsStillRunnable()) {
        Terminate();
        return;
    }

    start_time = GetWorld().GetFrame();
    while (IsStillRunnable() && !ptc_boss->IsReadyForPhaseChange()) {
        AIAction_DoNothing();
        timer = GetWorld().GetFrame() - start_time;
        if (timer / CMainGameComponent::GetConstantFPS() > 3) {
            if (GFRandom(100, GetWorld().GetWorldSeed()) < 33) {
                AIAction_PerformAction(CCreatureAction_PlayCombatAnimation(GetCreature(), "STUMBLE", true));
                start_time = GetWorld().GetFrame();
            }
        }
    }

    if (!IsStillRunnable()) {
        Terminate();
        return;
    }

    if (ptc_boss->IsReadyForPhaseChange()) {
        ptc_boss->SetNextBossPhase(6, false);
        if (!IsStillRunnable()) {
            Terminate();
            return;
        }

        GetCreature().GetPhysicsTC()->SetFacingAngleXY(0.0f);
        GetCreature().GetPhysicsTC()->ResetPosition(PArenaCentre->GetPos());
        AIAction_PerformActionAndWait(CCreatureAction_JackDeathInto(GetCreature()), false);
        if (!IsStillRunnable()) {
            Terminate();
            return;
        }

        start_time = GetWorld().GetFrame();
        do {
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            AIAction_DoNothing();
            timer = GetWorld().GetFrame() - start_time;
        } while (timer < p_jack_def->DeathScreamTime * CMainGameComponent::GetConstantFPS());
    }
    Terminate();
}
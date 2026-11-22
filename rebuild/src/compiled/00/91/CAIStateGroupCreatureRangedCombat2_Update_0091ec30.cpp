// CAIStateGroup_CreatureRangedCombat2::Update()
// Leaf: ?Update@CAIStateGroup_CreatureRangedCombat2@@MAEXXZ
// Flags: default
#define FABLE_RETAIL_CREATURE_RANGED_COMBAT2_PASS27
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PUMPCOMBATACTIONS
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SQUARED2D_ONE_EXPRESSION_PASS23
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARE_CALLED
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_CTCCREATURENAVIGATION_ONEPOINTLOS
#define FABLE_RETAIL_CTCCARRYING_PLAYERHIT_PASS26
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_RESETAIMING
#define FABLE_RETAIL_CCOMBATMANAGER_UNSHEATH
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroup_CreatureRangedCombat2.h"
#include "engine/CAIBrain.h"
#include "engine/ThingAlive.h"
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/GFSqr.h"
#include "engine/CTCPerceiveThing.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/NCreatureMode.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCWeapon.h"
#include "engine/NCreatureAI.h"
#include "engine/EUnsheatheWeaponType.h"
#include "engine/CCombatManager.h"

inline bool CAIStateGroup_CreatureRangedCombat2::HasRangedWeaponOut() const
{
    return CTCCarrying::IsThingCarryingRangedWeapon(GetCreature());
}

void CAIStateGroup_CreatureRangedCombat2::Update()
{
    if (!IS_THING_ALIVE(PTargetThing)) {
        Terminate();
        return;
    }

    const CTCPerceiveThing* ptc_perceive;
    GetCreature().PeekTC(&ptc_perceive);
    const float FIRING_DISTANCE = ptc_perceive->GetExtendedSightRadius();
    EAIActionReturn ret = AI_ACTION_IN_PROGRESS;

    {
        CTCCreatureModeManager* ptc_cmm;
        if (GetCreature().GetTC(&ptc_cmm)) {
            ptc_cmm->RemoveMode(NCreatureMode::MODE_STRAFE);
            AddedStrafe = false;
        }
    }
    if (!HasRangedWeaponOut()) {
        GetCreature().GetCombatManager().CreatureUnsheathWeapon(GetCreature(), UNSHEATHE_RANGED_WEAPON);
        return;
    }

    const CTCCreatureNavigation* ptc_creature_nav;
    GetCreature().PeekTC(&ptc_creature_nav);
    bool is_target_reachable = ptc_creature_nav->IsAbleToNavigateFromPosToPos(GetCreature().GetPos(), PTargetThing->GetPos(), 2.0f);
    if (is_target_reachable) {
        do {
            if (!(IS_THING_ALIVE(PTargetThing) && NCreatureAI::IsBestTargetForAttack(GetCreature(), PTargetThing.Get()) && IsAbleToFire())) {
                Terminate();
                return;
            }
            if ((PTargetThing->GetPos() - GetCreature().GetPos()).GetSquaredMagnitude2D() < GFSqr(FIRING_DISTANCE * 0.95f)
                && ptc_creature_nav->IsPosNavLineOfSightTo(PTargetThing->GetPos())) {
                ret = AI_ACTION_FINISHED;
                break;
            }
            bool nav;
            ret = AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK, PTargetThing, 2.0f, nav, AI_MOVEMENT_RUNNING,
                                                true, false, true, true, 0.0f);
        } while (ret == AI_ACTION_IN_PROGRESS);
        if (ret == AI_ACTION_TERMINATE_PROCESS) {
            Terminate();
            return;
        }
    }

    {
        CTCCreatureModeManager* ptc_cmm;
        if (GetCreature().GetTC(&ptc_cmm)) {
            ptc_cmm->AddMode(NCreatureMode::MODE_STRAFE);
            AddedStrafe = true;
        }
    }

    const CTCCarrying* ptc_carrying;
    if (GetCreature().PeekTC(&ptc_carrying)) {
        CThing* pcarried = ptc_carrying->GetThingInPrimarySlot();
        CTCWeapon* ptc_weapon;
        if (IS_THING_ALIVE(pcarried) && pcarried->GetTC(&ptc_weapon) && ptc_weapon->IsProjectileWeapon())
            ptc_weapon->ResetAiming();
    }

    while (IS_THING_ALIVE(PTargetThing) && NCreatureAI::IsBestTargetForAttack(GetCreature(), PTargetThing.Get()) && IsAbleToFire()) {
        float sqr_sep_2d = (GetCreature().GetPos() - PTargetThing->GetPos()).GetSquaredMagnitude2D();
        if (is_target_reachable && sqr_sep_2d > GFSqr(FIRING_DISTANCE))
            return;
        if (!ptc_creature_nav->IsPosNavLineOfSightTo(PTargetThing->GetPos()))
            return;
        ret = AIAction_PumpCombatActions();
        if (ret == AI_ACTION_TERMINATE_PROCESS) {
            Terminate();
            return;
        }
    }
    Terminate();
}
// 0095bcf0 CAIStateGroup_HoverHornetQueen::Attack
// Flags: none
// Leaf: ?Attack@CAIStateGroup_HoverHornetQueen@@IAEXXZ
// Retail: 0x0095BCF0, 948 bytes, thiscall.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_GFFASTONEOVERSQRT_HOST_BITS_PASS23
#define FABLE_RETAIL_C3DVECTOR_SUBTRACT_XY_GROUP
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_GROUPED
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_COMBATMANAGER
#define FABLE_RETAIL_CWORLDMAP_HOVERQUEEN_QUERY_PASS23
#define FABLE_RETAIL_CCOMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER_PREFERREDPOS
#define FABLE_RETAIL_CCOMBATMANAGER_SETATTACKERREADY
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_WASPQUEENPRIMARYATTACK_PASS23
#define FABLE_RETAIL_CWASPQUEENBATTLEDEF_PASS23
#define FABLE_RETAIL_NCREATUREAI_WASP_ATTACK_PASS23
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE_COMBATPERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE
#define FABLE_RETAIL_CAISTATEGROUP_HOVERHORNETQUEEN_ATTACK_PASS23
#define FABLE_RETAIL_CAISTATEGROUP_HOVERMOVEMENTBASE_ATTACK_PASS23
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBaseDef.h"
#include "engine/_quarantine/CWaspQueenBattleDef.h"
#include "engine/CCreatureAction_WaspQueenPrimaryAttack.h"
#include "engine/CCombatManager.h"
#include "engine/CDefinitionManager.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/NCreatureAI.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_HoverHornetQueen.h"

void CAIStateGroup_HoverHornetQueen::Attack()
{
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    CCombatManager& combat_man = GetCreature().GetCombatManager();
    if (!GetCreature().IsInMeleeAsAttacker()) {
        CThingAICreature* creature = &GetCreature();
        combat_man.RegisterCombatant(*creature, *PTargetThing.Get(), true);
    }
    if (GetCreature().IsInMeleeAsAttacker()) {
        CThing* creature = &GetCreature();
        CWorld& world = GetWorld();
        world.GetCombatManager().SetAttackerAsReadyToAttack(static_cast<CThingCreatureBase&>(*creature), true);
    }
    EAIActionReturn ret;
    do {
        if (!PTargetThing.Get() || (PTargetThing.Get()->IsDead())) {
            Terminate();
            return;
        }
        if (CircleMarkers.size() > 0) {
            CIntelligentPointer<CThing>& first = CircleMarkers[0];
            if (!first.Get() || (first.Get()->IsDead())) {
                Terminate();
                return;
            }
        }
        if (IsStopping() || !GetCreature().IsInMeleeAsAttacker()) {
            Terminate();
            return;
        }
        if (!NCreatureAI::IsToAttackTarget(&GetCreature(), PTargetThing.Get())) {
            Terminate();
            return;
        }
        C3DVector target_pos = combat_man.GetPreferredPos(GetCreature());
        float preffered_dis = (target_pos - PTargetThing.Get()->GetPos()).GetMagnitude();
        target_pos.Z = GetWorld().GetWorldMap().GetGroundSizeZAt(target_pos);
        C3DVector direction = target_pos - PTargetThing.Get()->GetPos();
        direction.Normalise();
        target_pos = PTargetThing.Get()->GetPos() + direction * preffered_dis;
        bool nav;
        ret = AIAction_MoveToPosProximityAndFacePos(ACTION_REPEAT_CALLBACK, target_pos, 0.05f, PTargetThing.Get()->GetPos(), nav, AI_MOVEMENT_HOVER_MEDIUM);
    } while (ret == AI_ACTION_IN_PROGRESS);
    if (ret != AI_ACTION_FINISHED)
        return;
    if (!IsStillRunnable() || !GetCreature().IsInMeleeAsAttacker()) {
        Terminate();
        return;
    }
    {
        CThingAICreature* creature = &GetCreature();
        AIAction_CombatPerformActionAndWait(CCreatureAction_WaspQueenPrimaryAttack(*creature, *PTargetThing.Get()), false);
    }
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    {
    long pause_start_frame = GetWorld().GetFrame();
    CDefPointer<const CWaspQueenBattleDef> p_wasp_def;
    GetCreature().PeekBaseDef().GetSubDef(&p_wasp_def);
    const CWaspQueenBattleDef* d = p_wasp_def.operator->();
    if (d->NumFramesPauseBetweenAttacks > 0) {
        do {
            AIAction_LookAtThing(ACTION_REPEAT_CALLBACK, PTargetThing, 0x21, 0.0027777001f);
            if (!IsStillRunnable())
                break;
        } while (GetWorld().GetFrame() - pause_start_frame < d->NumFramesPauseBetweenAttacks);
    }
    }
    if (!IsStillRunnable())
        Terminate();
}
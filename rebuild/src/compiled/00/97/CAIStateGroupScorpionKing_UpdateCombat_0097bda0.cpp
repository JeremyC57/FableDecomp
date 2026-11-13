// CAIStateGroup_ScorpionKing::UpdateCombat() -- one combat step of the scorpion king: close and facing the enemy he charges into him; within his combat type's near range he lets the enemy's shield react, pumps his combat actions or turns with an unblockable claw attack, looks at the enemy and plunges his tail once he has lost enough health; further away he looks at the enemy, then charges (announcing it with a creature-charging event) or plunges his tail; when the enemy is knocked down he drops a claw obstruction in front of himself and waits two seconds
// Grade: RELOCATION_MATCH (2009 bytes, 624/624 instructions; VC7.1 RTM 13.10.3077, no flags)
// Leaf: ?UpdateCombat@CAIStateGroup_ScorpionKing@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0097BDA0, 2009 bytes (`sub esp,15Ch` .. `ret` at 0x0097C578; the next function UpdatePhase2 starts at
//   0x0097C580, so the catalog length is right). No EH frame (the TU has no /EHsc).
// Identity: Ego_r.pdb / FableWin.pdb ?UpdateCombat@CAIStateGroup_ScorpionKing@@IAEXXZ (protected); donor order IsStillRunnable / ... /
//   UpdateCombat / UpdatePhase2 / Update (Ego_r 0x009A0500 / 0x009A0B40 / 0x009A14A0 / 0x009A1500) = retail 0x0097B870 / 0x0097BDA0 /
//   0x0097C580 / 0x0097C5E0; the landed Update 0x0097C5E0 calls this address as UpdateCombat.
// Callees (PDB spellings): CDefClassBase::GetSubDef<CScorpionKingBattleDef> 0x008C5A50 (shared body, FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY,
//   out of line), IsStillRunnable 0x0097B870, GetWorld 0x008FCFC0, CWorld::GetFrame 0x0049D870, CVectorMap::LowerBound 0x0040F020 (the
//   expanded GetTC<CTCBoss / CTCPhysicalShieldSpell / CTCEntityEvents>), GetPItem 0x00A01B50, CThing::GetPos 0x004C73D0,
//   CTCPhysicsBase::GetFacingVector (slot +0x118), CCreatureAction_ScorpionChargeInto ctor 0x008C4E20 (its implicit dtor expanded at the
//   8.0f site, called at 0x0097C900 at the charge site), AIAction_PerformActionAndWait 0x0097DB70, ~CBaseIntelligentPointer 0x00A01C10,
//   ~CCombatActionBase 0x0062E900, Terminate 0x008FCF40, CThingCreatureBase::GetCombatTypeDef 0x00666D10 (FrontNearLimit +0x74),
//   IS_THING_ALIVE<CIntelligentPointer<CThingCreatureBase> > 0x006961F0, CTCPhysicalShieldSpell::AllowReactionsOnHitFromAttacker
//   0x0079E580, AIAction_PumpCombatActions 0x0097D980, the Right / Left turn attack ctors 0x0097C830 / 0x0097C760, the converting
//   CIntelligentPointer<const CThing> ctor 0x0091F990 (called at the 0.1f LookAt, expanded at the 0.02f LookAt: c2 inline scores 417 v 51),
//   AIAction_LookAtThing 0x0097D6F0, CCreatureAction_ScorpionPrepareTailPlunge ctor 0x0097C920, CBaseIntelligentPointer ctor / SetPItem
//   0x00A01B10 / 0x00A01B90, operator new 0x00BFEA1A, CTCEntityEvents::AddEvent 0x008113C0, CTCBoss::ResetStrikesSinceHitHero
//   0x007AED50, NCreatureMode::IsKnockedDown 0x0085F010 (the expanded HasEnemyBeenKnockedDown, retail twin 0x00954130 MazeBase),
//   CTCDPhysicalObstruction::Create 0x007FD4B0 / SetLifeTimeSeconds 0x007FD3D0, AIAction_DoNothing 0x0097E200.
// Constants: 6.0f 0x0125A2A0, 0.75f 0x01238174, 0.5f 0x0122F59C, 0.0f 0x0122DEDC, -0.5f 0x01238798, 0.2f 0x012305A0, 8.0f / 0.1f / 0.02f / 2.0f
//   pushed; GFastOneOverSqrtTable 0x013CE558; CMainGameComponent::ConstantFPS 0x01375550.
// Shape: the charge site 2 is open code, not an expanded Charge(): the Ego_r Charge (100 bytes) has no event, and c2 scores a 187-byte
//   Charge at 2348 there (T 74); a forced Charge expands its ChargeInto dtor (f 1.0) where retail calls 0x0097C900. The unused
//   `GetCreature().GetPos()` before the charging event (0x0097C2BD) is a dead named local (an unused call result; GetPos has no side effect
//   this TU sees: FableWin's out-of-line Charge(const C3DVector&, float) suggests it was that by-reference argument in the original).
//   Spellings fitted against retail (FITTED, see NOTES.md): the far arm tests the tail-strike case first (`if (ChargeCount >= N || Health <= (HL - D)) { tail plunge } else { charge }`): the reverse order makes c2 give the battle_def near piece ebx before boss (+16 bytes); `(HealthAtLastTailStrike - DamageToCauseTailStrike)` in parentheses gives retail's creature-pointer-first load order at both tail-strike tests (0x0097C15A / 0x0097C234); FABLE_RETAIL_C3DVECTOR_NORMALISE_UNPAREN gives the two `fstp st(0)` pops around the exponent shift (0x0097BE80 / 0x0097BE85); a by-value `C3DVector facing` with the side vector built in place, `to_enemy.Dot(C3DVector(facing.Y, -facing.X, 0.0f))` (retail multiplies to_enemy.Z by 0.0f at 0x0097C0A0, so a Z = 0 vector is dotted), gives retail's x87 operand orders at 0x0097BFF7..0x0097C011 / 0x0097C092 (c2 loads the factor whose field record is newer; the explicit facing.X / facing.Y references create those records first). The wait loop's frame difference is a named
//   local assigned at the end of a do-while: `(GetFrame() - start) / GetConstantFPS()` in the condition reads ConstantFPS into a
//   cpu reg (edi) before GetFrame (idiv edi) and loses the aligned loop head.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_GETCOMBATTYPEDEF
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_PUMPCOMBATACTIONS
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_SCORPIONKING
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_SCORPIONCHARGEINTO
#define FABLE_RETAIL_CCREATUREACTION_SCORPIONUNBLOCKABLELEFTTURNATTACK
#define FABLE_RETAIL_CCREATUREACTION_SCORPIONUNBLOCKABLERIGHTTURNATTACK
#define FABLE_RETAIL_CCREATUREACTION_SCORPIONPREPARETAILPLUNGE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_UNPAREN
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D_B
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCBOSS_STRIKES
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_ADDEVENT
#define FABLE_RETAIL_CTCPHYSICALSHIELDSPELL_ALLOWREACTIONS
#define FABLE_RETAIL_CTCDPHYSICALOBSTRUCTION_CREATE
#define FABLE_RETAIL_NCREATUREMODE_QUERIES
#define FABLE_RETAIL_CCOMBATTYPEDEF
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#include "engine/CAIStateGroupBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroup_ScorpionKing.h"
#include "engine/CCreatureAction_ScorpionChargeInto.h"
#include "engine/CCreatureAction_ScorpionUnblockableLeftTurnAttack.h"
#include "engine/CCreatureAction_ScorpionUnblockableRightTurnAttack.h"
#include "engine/CCreatureAction_ScorpionPrepareTailPlunge.h"
#include "engine/CTCBoss.h"
#include "engine/CTCPhysicsBase.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBaseDef.h"
#include "engine/CAIBrain.h"
#include "engine/CDefPointer.h"
#include "engine/CDefClassBase.h"
#include "engine/CScorpionKingBattleDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CCombatTypeDef.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CTCEntityEvents.h"
#include "engine/NEntityEvents_CEventCreatureCharging.h"
#include "engine/CTCPhysicalShieldSpell.h"
#include "engine/CTCDPhysicalObstruction.h"
#include "engine/NCreatureMode.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/ThingAlive.h"
#include "engine/EAIAction.h"

bool CAIStateGroup_ScorpionKing::HasEnemyBeenKnockedDown()
{
    return NCreatureMode::IsKnockedDown(*PEnemy);
}

void CAIStateGroup_ScorpionKing::UpdateCombat()
{
    CDefPointer<const CScorpionKingBattleDef> battle_def;
    GetCreature().PeekBaseDef().GetSubDef(&battle_def);
    if (IsStillRunnable()) {
        EnemyKnockDownFrame = GetWorld().GetFrame();
        CTCBoss* boss;
        GetCreature().GetTC(&boss);
        C3DVector to_enemy = PEnemy->GetPos() - GetCreature().GetPos();
        float distance = to_enemy.GetMagnitude2D();
        to_enemy.Normalise();
        if (distance < 6.0f && GetCreature().GetPhysicsTC()->GetFacingVector().Dot(to_enemy) > 0.75f) {
            ++ChargeCount;
            AIAction_PerformActionAndWait(CCreatureAction_ScorpionChargeInto(GetCreature(), *PEnemy, 8.0f), false);
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
        } else if (distance < GetCreature().GetCombatTypeDef().FrontNearLimit) {
            boss->SetCountingStrikesSinceHitHero(false);
            C3DVector facing = GetCreature().GetPhysicsTC()->GetFacingVector();
            float facing_dot = facing.Dot(to_enemy);
            if (facing_dot > 0.5f) {
                if (IS_THING_ALIVE(PEnemy)) {
                    CTCPhysicalShieldSpell* shield;
                    if (PEnemy->GetTC(&shield))
                        shield->AllowReactionsOnHitFromAttacker(&GetCreature(), true);
                }
                AIAction_PumpCombatActions();
                if (!IsStillRunnable()) {
                    Terminate();
                    return;
                }
            } else {
                EAIActionReturn result;
                if (to_enemy.Dot(C3DVector(facing.Y, -facing.X, 0.0f)) > 0.0f)
                    result = AIAction_PerformActionAndWait(CCreatureAction_ScorpionUnblockableRightTurnAttack(GetCreature(), *PEnemy), false);
                else
                    result = AIAction_PerformActionAndWait(CCreatureAction_ScorpionUnblockableLeftTurnAttack(GetCreature(), *PEnemy), false);
                if (!IsStillRunnable()) {
                    Terminate();
                    return;
                }
                if (result != AI_ACTION_FINISHED)
                    return;
                if (facing_dot > -0.5f)
                    result = AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED, PEnemy, 0x11, 0.1f);
                if (!IsStillRunnable()) {
                    Terminate();
                    return;
                }
                if (result != AI_ACTION_FINISHED)
                    return;
                if (GetCreature().GetHealth() <= (HealthAtLastTailStrike - battle_def->DamageToCauseTailStrike)) {
                    AIAction_PerformActionAndWait(CCreatureAction_ScorpionPrepareTailPlunge(GetCreature(), *PEnemy), false);
                    if (!IsStillRunnable()) {
                        Terminate();
                        return;
                    }
                    HealthAtLastTailStrike = GetCreature().GetHealth();
                    boss->ResetStrikesSinceHitHero();
                    ChargeCount = 0;
                }
            }
        } else {
            boss->SetCountingStrikesSinceHitHero(true);
            EAIActionReturn result = AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED, PEnemy, 0x11, 0.02f);
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            if (result == AI_ACTION_FINISHED) {
                if (ChargeCount >= battle_def->NumChargesBeforeTailStrike ||
                    GetCreature().GetHealth() <= (HealthAtLastTailStrike - battle_def->DamageToCauseTailStrike)) {
                    AIAction_PerformActionAndWait(CCreatureAction_ScorpionPrepareTailPlunge(GetCreature(), *PEnemy), false);
                    if (!IsStillRunnable()) {
                        Terminate();
                        return;
                    }
                    HealthAtLastTailStrike = GetCreature().GetHealth();
                    boss->ResetStrikesSinceHitHero();
                    ChargeCount = 0;
                } else {
                    to_enemy = PEnemy->GetPos() - GetCreature().GetPos();
                    distance = to_enemy.GetMagnitude2D();
                    C3DVector charge_start = GetCreature().GetPos();
                    CTCEntityEvents* events;
                    if (GetCreature().GetTC(&events))
                        events->AddEvent(new NEntityEvents::CEventCreatureCharging());
                    ++ChargeCount;
                    result = AIAction_PerformActionAndWait(CCreatureAction_ScorpionChargeInto(GetCreature(), *PEnemy, distance), false);
                    if (!IsStillRunnable()) {
                        Terminate();
                        return;
                    }
                    if (result != AI_ACTION_FINISHED)
                        return;
                }
            }
        }
    }
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    if (HasEnemyBeenKnockedDown()) {
        long start_frame = GetWorld().GetFrame();
        float radius = battle_def->ClawObstructionRadius + 0.2f;
        C3DVector position = GetCreature().GetPos() + GetCreature().GetPhysicsTC()->GetFacingVector() * radius;
        CThing* obstruction = CTCDPhysicalObstruction::Create(position, position, battle_def->ClawObstructionRadius);
        if (obstruction) {
            CTCDPhysicalObstruction* driver;
            obstruction->GetTCDriver(&driver);
            driver->SetLifeTimeSeconds(2.0f);
        }
        long frames_waited;
        do {
            AIAction_DoNothing();
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            frames_waited = GetWorld().GetFrame() - start_frame;
        } while (frames_waited / CMainGameComponent::GetConstantFPS() < 2);
    }
}
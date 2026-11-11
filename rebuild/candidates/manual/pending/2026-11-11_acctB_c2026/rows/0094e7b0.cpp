// CAIStateGroup_EarthTroll::Update() -- one combat round of the earth troll: rise up if needed, terminate unless still runnable; with the enemy alive, measure the 2D distance to its predicted position; inside TROLL_MIN_THROWING_DISTANCE turn towards it when it is not ahead (TURNING_AC90 / TURNING_C90 by the side), then ground-pound when still close and wait for the action, noting a knock-down; between the two ranges look at the enemy and throw rocks (combo reset and handedness on each new volley, a SPECIAL_IDLE pause after the second shot) while it stays in range and is not knocked down; out of range terminate; then let the current action finish and boast (SPECIAL_BOAST) when the enemy was knocked down far enough away and recently enough (CTrollBattleDef); terminate when the enemy is dead or the group is stopping
// Leaf: ?Update@CAIStateGroup_EarthTroll@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1 RTM) @ 0x0094E7B0, 1980 bytes, plain `ret`, frame 0x15C (extent checked: int3 padding before, last `ret`
//   at 0x0094EF6B, next function 0x0094EF70). Ego_r.pdb / FableWin.pdb ?Update@CAIStateGroup_EarthTroll@@MAEXXZ (protected virtual; Ego_r donor
//   0x00972EC0 right after IsRunnable 0x00972D90 = retail 0x0094E6B0) = slot 12 of vtable 0x0128C044 (rtti-vtables.tsv): the catalog name is right.
// Callees (each checked against retail with tmp/rel.py): CAIStateGroup_TrollBase::ForceRiseUp 0x0094DBC0 / IsStillRunnable 0x0094D930 /
//   GetEnemyPredictedPos 0x0094D680 / HasEnemyBeenKnockedDown 0x0094D9C0 / ThrowRocks 0x0094DC40, CAIStateGroupBase::Terminate 0x008FCF40 /
//   GetWorld 0x008FCFC0 / IsStopping 0x008FD010 / AIAction_PerformAction 0x0097D8B0 / AIAction_PerformActionAndWait 0x0097DB70 /
//   AIAction_LookAtThing 0x0097D6F0 / AIAction_DoNothing 0x0097E200, CDefClassBase::GetSubDef<CTrollBattleDef> 0x008C78F0 (on
//   CThing::PBaseDef +0x70), CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::GetPos 0x004C73D0, GFastOneOverSqrtTable 0x013CE558 (the
//   expanded fast Normalise; 3.0f 0x0122DED4, 0.5f 0x0122F59C), CTCPhysicsBase::GetFacingVector (virtual +0x118 on CThing::PhysicsTC),
//   C3DVector::RotateXY 0x00A14040, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CCreatureAction_PlayCombatAnimation
//   0x008B7FF0, CCreatureAction_TrollGroundPound 0x0094E5F0, ~CCombatActionBase 0x0062E900, CThingCreatureBase::IsPerformingAnyAction
//   0x006622A0 / SetLastActionHandedness 0x006619F0, CSuspendableProcessMicrothread::SuspendSuspendableProcess (virtual +0x08 on PBrain),
//   CWorld::GetFrame 0x0049D870, CVectorMap::LowerBound 0x0040F020 (the expanded GetTC<CTCCombat>, TCI 0x30), CTCCombat::ResetComboChainPos
//   0x00733210, CIntelligentPointer<const CThing>(CIntelligentPointer<CThingCreatureBase>&) 0x0091F990 (the by-value LookAtThing argument).
//   Literals: "TURNING_AC90" 0x012697D8, "TURNING_C90" 0x012697CC, "SPECIAL_BOAST" 0x0127A550, "SPECIAL_IDLE" 0x0127A43C, 0.2f pushed, -0.25f pushed.
// Shape: (1) the throwing range is an `else if (distance <= TROLL_MAX_THROWING_DISTANCE)` arm with a final `else { Terminate(); return; }`:
//   VC7.1 then lays the shared idle / boast tail right after the close-range arm and the throwing arm after it, jumping back into the tail
//   (0x0094EEB8 `jne 0x0094EB91`); a plain `else { if (distance > MAX) { Terminate(); return; } ... }` keeps source order (tail last).
//   (2) The closing `if (IsStopping()) { Terminate(); return; }` keeps its own release copy (0x0094EF36) beside the natural exit's
//   (0x0094EF51); without the `return` the two merge and every exit block before them is cross-jumped differently. (3) The ground pound
//   is written out (`AIAction_PerformAction(CCreatureAction_TrollGroundPound(GetCreature(), *PEnemy))`): the Ego_r.pdb helper
//   CAIStateGroup_TrollBase::GroundPound (donor 0x009722C0, no retail copy) defined in this TU is not expanded by VC7.1 at this site (c2 inline
//   trace: c1 = 62, site frequency 0.023, score 431 against the 74 threshold), so retail's expanded bytes come from the direct call.
//   (4) The visible GetSubDef<T> body (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY, noinline) gives the `dec [ecx+4]` releases; the 2D distance is
//   C3DVector::GetMagnitude2D's two-statement body (X squared first, 0x0094E886); the one-expression form squares Y first.
// Layout: shared headers with the chunk 2026 overlay (hdr/README.md): retail CAIStateGroup_TrollBase / CAIStateGroup_EarthTroll classes
//   (Ego_r layout and spellings), C3DVector MAGNITUDE2D, CIntelligentPointer CONVERT_MUTABLE_OUTOFLINE, CThingCreatureBase SETHANDEDNESS,
//   CTCCombat RESETCOMBO, CCreatureAction_TrollGroundPound CTOR; CAIStateGroupBase.h is the chunk 2017 copy (PERFORMACTION_NOWAIT).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCOMBATABILITY
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE_OUTOFLINE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CTHINGCREATUREBASE_SETHANDEDNESS
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION_NOWAIT
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_SUSPENDABLE_BASE
#define FABLE_RETAIL_CAISTATEGROUP_TROLLBASE
#define FABLE_RETAIL_CAISTATEGROUP_EARTHTROLL
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_PLAYCOMBATANIMATION
#define FABLE_RETAIL_CCREATUREACTION_TROLLGROUNDPOUND_CTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_RESETCOMBO
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/CAIStateGroupBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroup_TrollBase.h"
#include "engine/CAIStateGroup_EarthTroll.h"
#include "engine/CCreatureAction_TrollGroundPound.h"
#include "engine/CTCCombat.h"
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
#include "engine/CTrollBattleDef.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_PlayCombatAnimation.h"
#include "engine/CWorld.h"
#include "engine/ThingAlive.h"

void CAIStateGroup_EarthTroll::Update()
{
    ForceRiseUp();
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    bool knocked_down = false;
    CDefPointer<const CTrollBattleDef> troll_def;
    GetCreature().PeekBaseDef().GetSubDef(&troll_def);
    if (IS_THING_ALIVE(PEnemy)) {
        if (!IsStillRunnable()) {
            Terminate();
            return;
        }
        C3DVector offset = GetEnemyPredictedPos() - GetCreature().GetPos();
        float distance = offset.GetMagnitude2D();
        if (distance < TROLL_MIN_THROWING_DISTANCE) {
            C3DVector direction = offset;
            direction.Normalise();
            if (GetCreature().GetPhysicsTC()->GetFacingVector().Dot(direction) < 0.5f) {
                C3DVector right = GetCreature().GetPhysicsTC()->GetFacingVector();
                right.RotateXY(-0.25f);
                if (right.Dot(direction) > 0.0f)
                    AIAction_PerformActionAndWait(CCreatureAction_PlayCombatAnimation(GetCreature(), "TURNING_AC90", true), false);
                else
                    AIAction_PerformActionAndWait(CCreatureAction_PlayCombatAnimation(GetCreature(), "TURNING_C90", true), false);
                if (!IsStillRunnable()) {
                    Terminate();
                    return;
                }
                offset = GetEnemyPredictedPos() - GetCreature().GetPos();
                distance = offset.GetMagnitude2D();
            }
            if (distance < TROLL_MIN_THROWING_DISTANCE) {
                EnemyKnockDownFrame = GetWorld().GetFrame();
                AIAction_PerformAction(CCreatureAction_TrollGroundPound(GetCreature(), *PEnemy));
                while (GetCreature().IsPerformingAnyAction()) {
                    if (!IsStillRunnable()) {
                        Terminate();
                        return;
                    }
                    if (HasEnemyBeenKnockedDown())
                        knocked_down = true;
                    PBrain->SuspendSuspendableProcess();
                }
                if (!IsStillRunnable()) {
                    Terminate();
                    return;
                }
            }
            offset = PEnemy->GetPos() - GetCreature().GetPos();
            distance = offset.GetMagnitude2D();
        } else if (distance <= TROLL_MAX_THROWING_DISTANCE) {
            AIAction_LookAtThing((EAIActionRepeat)1, PEnemy, 0x11, 0.2f);
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            ShotCounter = 0;
            EAIActionReturn result;
            do {
                if (ShotCounter == 0) {
                    EnemyKnockDownFrame = GetWorld().GetFrame();
                    CTCCombat* combat;
                    GetCreature().GetTC(&combat);
                    combat->ResetComboChainPos();
                    GetCreature().SetLastActionHandedness((ECombatAnimationHandedness)0);
                }
                result = ThrowRocks();
                if (!IsStillRunnable()) {
                    Terminate();
                    return;
                }
                if (HasEnemyBeenKnockedDown())
                    knocked_down = true;
                if (ShotCounter == 2 && result == 2) {
                    CCreatureAction_PlayCombatAnimation idle(GetCreature(), "SPECIAL_IDLE", true);
                    while (GetCreature().IsPerformingAnyAction()) {
                        if (!IsStillRunnable()) {
                            Terminate();
                            return;
                        }
                        if (HasEnemyBeenKnockedDown())
                            knocked_down = true;
                        PBrain->SuspendSuspendableProcess();
                    }
                    ShotCounter = 0;
                    if (!knocked_down)
                        result = AIAction_PerformActionAndWait(idle, false);
                }
                if (!IsStillRunnable()) {
                    Terminate();
                    return;
                }
                offset = PEnemy->GetPos() - GetCreature().GetPos();
                distance = offset.GetMagnitude2D();
                if (HasEnemyBeenKnockedDown())
                    knocked_down = true;
            } while ((result == 4 || result == 2) && distance >= TROLL_MIN_THROWING_DISTANCE && distance <= TROLL_MAX_THROWING_DISTANCE
                     && !knocked_down);
            if (distance > TROLL_MAX_THROWING_DISTANCE) {
                Terminate();
                return;
            }
        } else {
            Terminate();
            return;
        }
        while (GetCreature().IsPerformingAnyAction()) {
            AIAction_DoNothing();
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
        }
        long frames = GetWorld().GetFrame() - EnemyKnockDownFrame;
        if (!IsStopping() && distance >= troll_def->MinLaughDistance && knocked_down && frames < troll_def->MaxTimeBetweenKnockdownAndBoast)
            AIAction_PerformActionAndWait(CCreatureAction_PlayCombatAnimation(GetCreature(), "SPECIAL_BOAST", true), false);
    } else
        Terminate();
    if (IsStopping()) {
        Terminate();
        return;
    }
}

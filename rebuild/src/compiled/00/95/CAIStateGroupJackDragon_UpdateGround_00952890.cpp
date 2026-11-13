// CAIStateGroup_JackDragon::UpdateGround() -- one ground-phase step of the Jack-dragon boss: after MaxTimeOnGround fly out of the ground; in snap range snap at a hitting enemy, snap fast after facing it for MaxTimeBeforeSnap, or rear up and roar once per MinTimeBetweenSlams; at long range fly out after MaxTimeAtLongRangeBeforeFlyAway or MaxHitsBeforeFlyAway boss hits (once the battle started), rear up within 25 units, or roar on landing; terminate when the enemy is gone or the group stops
// Leaf: ?UpdateGround@CAIStateGroup_JackDragon@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1 RTM) @ 0x00952890, 1509 bytes, plain `ret`, frame 0x148. Ego_r.pdb ?UpdateGround@CAIStateGroup_JackDragon@@AAEXXZ
//   (private; donor 0x00977280 between UpdateHover 0x00976950 and Update 0x00977A00 = retail 0x00951FB0 / 0x00952E80; the landed Update
//   dispatches the ground state here).
// Callees (all checked against retail with rel.py): GetSubDef<CJackDragonDef> 0x008E3300, GetPItem 0x00A01B50, CThing::GetPos 0x004C73D0,
//   GFastOneOverSqrtTable 0x013CE558 (the expanded fast Normalise), CAIStateGroupBase::GetWorld 0x008FCFC0 / AIAction_PerformActionAndWait
//   0x0097DB70 / AIAction_DoNothing 0x0097E200 / IsStopping 0x008FD010 / Terminate 0x008FCF40, CWorld::GetFrame 0x0049D870,
//   CMainGameComponent::ConstantFPS 0x01375550, SetHeadTurning 0x00950BB0, WaitForAnimLoop(float) 0x00950950, FlyOutOfGround 0x00951B20,
//   FindHitEvent 0x00950B20, IsStillRunnable 0x0094FDF0, StayIdle(float) 0x0094FE30, CTCPhysicsBase::GetFacingVector (virtual +0x118),
//   CVectorMap::LowerBound 0x0040F020 (the expanded GetTC<CTCBoss>), action ctors CCreatureAction_SnapDragon 0x009531C0 /
//   SnapDragonFast 0x00953260 / DragonRearUpFull 0x00953120 / DragonRoar 0x00953300, ~CCombatActionBase 0x0062E900 and the implicit
//   ~CCreatureAction_DragonRearUpFull 0x009531A0.
// Constants: 3.0f 0x0122DED4 / 0.5f 0x0122F59C (fast inverse square root), cos 15 deg 0x01293364, 25.0f 0x0123003C.
// Shape: the def comes from the shared GetSubDef body (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY, noinline): VC7.1 then keeps the def in edi
//   and releases it with no null test, as retail. The distance is GetMagnitude2D (fsqrt) of the unnormalised offset, then the fast
//   Normalise shares its squares. The action temporaries go straight into AIAction_PerformActionAndWait(..., false) (the `push 0`
//   before the ctor). Exit layout: IsStillRunnable (0x0094FDF0, the same TU) is defined here, called at every in-branch check and
//   expanded by VC7.1 itself at the final one; the final check is `if (!IsStillRunnable()) { Terminate(); return; }` -- with the
//   return the natural exit's release is merged into the middle Terminate copy (0x00952BD7) as in retail; without it the end copy
//   survives (4 bytes short, `je` short).
// Layout: overlay opt-ins (hdr/README.md): CAIStateGroup_JackDragon (retail class), CJackDragonDef, CCreatureAction_SnapDragon /
//   SnapDragonFast / DragonRearUpFull / DragonRoar (new header), CTCBoss GENERICCOUNTER (FableWin ?GetGenericCounter@CTCBoss@@QBEJXZ).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
#define FABLE_RETAIL_UPDATE_NEAREST_INTERESTING
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CJACKDRAGONDEF
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CCREATUREACTION_SNAPDRAGON
#define FABLE_RETAIL_CCREATUREACTION_SNAPDRAGONFAST
#define FABLE_RETAIL_CCREATUREACTION_DRAGONREARUPFULL
#define FABLE_RETAIL_CAISTATEGROUP_JACKDRAGON
#define FABLE_RETAIL_CAISTATEGROUP_JACKDRAGON_UPDATEGROUND
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCBOSS_GENERICCOUNTER
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingBaseDef.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CDefinitionManager.h"
#include "engine/CJackDragonDef.h"
#include "engine/CCreatureAction_SnapDragon.h"
#include "engine/CCreatureAction_SnapDragonFast.h"
#include "engine/CCreatureAction_DragonRearUpFull.h"
#include "engine/CCreatureAction_DragonRoar.h"
#include "engine/CAIStateGroup_JackDragon.h"
#include "engine/CWorld.h"
#include "engine/CTCBoss.h"
#include "engine/CMainGameComponent.h"
#include "engine/ThingAlive.h"

// The same-TU IsStillRunnable (retail 0x0094FDF0: alive enemy and not stopping), visible so VC7.1 can expand it at the final check.
bool CAIStateGroup_JackDragon::IsStillRunnable()
{
    return IS_THING_ALIVE(PEnemy) && !IsStopping();
}

void CAIStateGroup_JackDragon::UpdateGround()
{
    CDefPointer<const CJackDragonDef> def;
    GetCreature().PeekBaseDef().GetSubDef(&def);
    C3DVector to_enemy = PEnemy->GetPos() - GetCreature().GetPos();
    float distance = to_enemy.GetMagnitude2D();
    to_enemy.Normalise();
    long frames_on_ground = GetWorld().GetFrame() - FrameWhenLanded;
    SetHeadTurning(true);
    if (frames_on_ground > CMainGameComponent::GetConstantFPS() * def->MaxTimeOnGround) {
        SetHeadTurning(false);
        WaitForAnimLoop(0.5f);
        FlyOutOfGround();
    } else if (distance < def->MaxSnapDistance) {
        BattleTriggered = true;
        long frames_since_slam = GetWorld().GetFrame() - FrameOfLastSlam;
        FrameWhenLastInRange = GetWorld().GetFrame();
        if (FindHitEvent() && !IsRangeAttack) {
            AIAction_PerformActionAndWait(CCreatureAction_SnapDragon(GetCreature(), *PEnemy.Get()), false);
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            StayIdle(1.0f);
        } else if (distance > def->MinSnapDistance &&
                   to_enemy.Dot(GetCreature().GetPhysicsTC()->GetFacingVector()) > 0.9659259915351868f) {
            if (++FramesInSnapRange > CMainGameComponent::GetConstantFPS() * def->MaxTimeBeforeSnap) {
                AIAction_PerformActionAndWait(CCreatureAction_SnapDragonFast(GetCreature(), *PEnemy.Get()), false);
                if (!IsStillRunnable()) {
                    Terminate();
                    return;
                }
                StayIdle(1.0f);
                FramesInSnapRange = 0;
            } else {
                AIAction_DoNothing();
            }
        } else if (frames_since_slam > CMainGameComponent::GetConstantFPS() * def->MinTimeBetweenSlams) {
            SetHeadTurning(false);
            WaitForAnimLoop(0.5f);
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            FramesInSnapRange = 0;
            AIAction_PerformActionAndWait(CCreatureAction_DragonRearUpFull(GetCreature(), *PEnemy.Get()), false);
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            StayIdle(0.5f);
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            AIAction_PerformActionAndWait(CCreatureAction_DragonRoar(GetCreature(), *PEnemy.Get()), false);
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            SetHeadTurning(true);
            StayIdle(2.0f);
            FrameOfLastSlam = GetWorld().GetFrame();
        } else {
            FramesInSnapRange = 0;
            AIAction_DoNothing();
        }
    } else {
        long frames_since_slam = GetWorld().GetFrame() - FrameOfLastSlam;
        long frames_since_in_range = GetWorld().GetFrame() - FrameWhenLastInRange;
        CTCBoss* boss;
        GetCreature().GetTC(&boss);
        float fps = (float)CMainGameComponent::GetConstantFPS();
        if ((frames_since_in_range > fps * def->MaxTimeAtLongRangeBeforeFlyAway ||
             boss->GetGenericCounter() > def->MaxHitsBeforeFlyAway) && BattleTriggered) {
            FrameOfLastSlam = GetWorld().GetFrame();
            SetHeadTurning(false);
            WaitForAnimLoop(0.5f);
            FlyOutOfGround();
        } else if (distance < 25.0f && frames_since_slam > fps * def->MinTimeBetweenSlams) {
            SetHeadTurning(false);
            WaitForAnimLoop(0.5f);
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            BattleTriggered = true;
            FrameWhenLastInRange = GetWorld().GetFrame();
            AIAction_PerformActionAndWait(CCreatureAction_DragonRearUpFull(GetCreature(), *PEnemy.Get()), false);
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            SetHeadTurning(true);
            StayIdle(0.5f);
            FrameOfLastSlam = GetWorld().GetFrame();
        } else if (frames_on_ground <= 2 && distance > def->MinRoarDistance) {
            SetHeadTurning(false);
            WaitForAnimLoop(0.5f);
            AIAction_PerformActionAndWait(CCreatureAction_DragonRoar(GetCreature(), *PEnemy.Get()), false);
            SetHeadTurning(true);
            StayIdle(1.0f);
        } else {
            AIAction_DoNothing();
        }
    }
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
}
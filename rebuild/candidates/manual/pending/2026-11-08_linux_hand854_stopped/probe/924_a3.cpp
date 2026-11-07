// CAIStateGroup_KrakenAttack::Update() -- the kraken boss fight loop: wait for the kraken to be triggered, then rise when every tentacle has been hit, pose and strike until the head is hit or idle too long, roar and high-attack after the low strikes run out, and sink back under water when told
// Leaf: ?Update@CAIStateGroup_KrakenAttack@@MAEXXZ
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
#define FABLE_RETAIL_CTHINGAICREATURE_IMMUNETOATTACK
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_COMBATPERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_SETCOMBATANIMATION
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CDefPointer.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CSuspendableProcessMicrothread.h"
#include "stlport/fable_containers.h"

enum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2, AI_ACTION_ABORTED = 3, AI_ACTION_RUNNING = 4 };

class CKrakenDef : public CSubDefClassBase {             // RTTI .?AVCKrakenDef@@ : CSubDefClassBase (Ego_r offsets = retail)
public:
    float HeightOffset;                                  // +0x28
    float PoolRadius;                                    // +0x2c
    float HeadRiseDelay;                                 // +0x30
    float HeadIdleTime;                                  // +0x34
    long LowStrikesBeforeHigh;                           // +0x38
};

class CTCBoss : public CTCBase {                         // RTTI .?AVCTCBoss@@ : CTCBase
public:
    void SetNextBossPhase(long phase, bool immediate);   // 0x007AEDA0 ?SetNextBossPhase@CTCBoss@@QAEXJ_N@Z
};

class CTCKraken : public CTCBase {                       // RTTI .?AVCTCKraken@@ : CTCBase; layout as the landed SetMode 0x007FC730
public:
    enum EKrakenMode { KRAKEN_MODE_HEAD_ATTACK = 3 };
    enum EKrakenBossPhase { KRAKEN_BOSS_PHASE_NONE = 0 };
    bool HaveAllTentaclesBeenHit() const;                // 0x007FB240
    bool IsTargetAlive() const;                          // 0x007FB120 (descriptive: IS_THING_ALIVE(PTarget), in neither PDB name set)
    const CKrakenDef& GetKrakenDef() const { return *PKrakenDef; }
    CThing* GetPTarget() const { return PTarget.Get(); }
    bool IsTriggered() const { return Phase != KRAKEN_BOSS_PHASE_NONE; }
    void SetHeadTracking(bool on) { HeadTrackTarget = on; }
    bool GetHeadHasBeenHit() const { return HeadHasBeenHit; }
    void SetHeadHasBeenHit(bool hit) { HeadHasBeenHit = hit; }
    bool ShouldReturnHeadToWater() const { return ReturnHeadToWater; }
private:
    void SetMode(EKrakenMode mode);                      // 0x007FC730
    friend class CAIStateGroup_KrakenAttack;
public:
    CDefPointer<const CKrakenDef>           PKrakenDef;          // +0x0c
    CIntelligentPointer<CThing>             PChamber;            // +0x10
    CIntelligentPointer<CThing>             PCentreMarker;       // +0x18
    CIntelligentPointer<CThing>             PTarget;             // +0x20
    CIntelligentPointer<CThing>             PHeadEffect;         // +0x28
    std::list<CIntelligentPointer<CThing> > ExplosionList;       // +0x30
    CIntelligentPointer<CThing>             PBeamWeaponEffect;   // +0x34
    long                                    DelayBeforeBeam;     // +0x3c
    CTCBoss*                                PTCBoss;             // +0x40
    std::list<CIntelligentPointer<CThing> > Tentacles;           // +0x44
    void*                                   AttackingTentacleIterator;   // +0x48 (list iterator)
    long                                    TentacleStrikeCounter;       // +0x4c
    bool                                    TentacleReadyToStrike;       // +0x50
    bool                                    HeadTrackTarget;             // +0x51
    bool                                    HeadHasBeenHit;              // +0x52
    long                                    LastStrikeTick;              // +0x54
    long                                    KillBeamCounter;             // +0x58
    bool                                    ReturnHeadToWater;           // +0x5c
    EKrakenBossPhase                        Phase;                       // +0x60
    EKrakenMode                             Mode;                        // +0x64
};

class CSoundPair { public: CSoundPair() { Sound[0] = 0; Sound[1] = 0; } long Sound[2]; };   // ??0CSoundPair@@QAE@XZ (inline)

class CCreatureAction_KrakenRiseUp : public CCombatActionBase {
public:
    CCreatureAction_KrakenRiseUp(CThingCreatureBase& creature, CThing& target);         // 0x00925330
};
class CCreatureAction_KrakenLowPoseInto : public CCombatActionBase {
public:
    CCreatureAction_KrakenLowPoseInto(CThingCreatureBase& creature, CThing& target);    // 0x00925440
};
class CCreatureAction_KrakenHighAttackInto : public CCombatActionBase {
public:
    CCreatureAction_KrakenHighAttackInto(CThingCreatureBase& creature, CThing& target); // 0x00925700
};
class CCreatureAction_KrakenRoarInto : public CCombatActionBase {
public:
    CCreatureAction_KrakenRoarInto(CThingCreatureBase& creature);                       // 0x009258A0
};
class CCreatureAction_KrakenReturnToWater : public CCombatActionBase {   // vtable 0x012906F4
public:
    CCreatureAction_KrakenReturnToWater(CThingCreatureBase& creature)
        : CCombatActionBase(creature, 250, CSoundPair())
    {
        SetCombatAnimation("RETURN_UNDERWATER");
    }
    virtual void Initialise();
    virtual void FinishAction();
    virtual CCreatureActionBase* Clone() const;
};

class CAIStateGroup_KrakenAttack : public CAIStateGroupBase {   // RTTI : CAIStateGroupBase
protected:
    virtual void Update();
};

void CAIStateGroup_KrakenAttack::Update()
{
    CThingAICreature& creature = GetCreature();
    CTCKraken* kraken;
    creature.GetTC(TCI_KRAKEN, &kraken);
    creature.SetAsImmuneToAttack(true);
    static bool s_DebugRegistered = false;               // 0x013BB68E
    static bool s_DebugSkipPhase = false;                // 0x013BB68D
    if (!s_DebugRegistered)
        s_DebugRegistered = true;
    while (!kraken->IsTriggered()) {
        if (AIAction_DoNothing() == AI_ACTION_STOPPING) {
            Terminate();
            return;
        }
        if (s_DebugSkipPhase) {
            CTCBoss* boss;
            GetCreature().GetTC(TCI_BOSS, &boss);
            boss->SetNextBossPhase(1, false);
            s_DebugSkipPhase = false;
            break;
        }
    }
    long attacksLeft = kraken->GetKrakenDef().LowStrikesBeforeHigh;
    long phase = 0;
    long startFrame = 0;
    float startHealth = 0.0f;
    bool first = true;
    for (;;) {
        switch (phase) {
        case 0:
            if (kraken->HaveAllTentaclesBeenHit()) {
                long end = GetWorld().GetFrame() + (long)(CMainGameComponent::GetConstantFPS() * kraken->GetKrakenDef().HeadRiseDelay);
                while (GetWorld().GetFrame() < end && !IsStopping())
                    reinterpret_cast<CSuspendableProcessMicrothread*>(PBrain)->SuspendSuspendableProcess();   // RTTI CAIBrain : CSuspendableProcessMicrothread@0 (slot 2)
                CThing* target = kraken->GetPTarget();
                if (target && !target->IsDead()) {
                    if (AIAction_PerformActionAndWait(CCreatureAction_KrakenRiseUp(GetCreature(), *kraken->GetPTarget()), false) != AI_ACTION_DONE) {
                        Terminate();
                        return;
                    }
                }
                phase = 2;
                kraken->SetMode(CTCKraken::KRAKEN_MODE_HEAD_ATTACK);
                kraken->SetHeadTracking(true);
                startFrame = CWorld::GetFrame();
                startHealth = GetCreature().GetHealth();
            } else {
                AIAction_DoNothing();
            }
            break;
        case 1:
            if (GetCreature().GetHealth() < startHealth
                || CWorld::GetFrame() - startFrame > (long)(CMainGameComponent::GetConstantFPS() * kraken->GetKrakenDef().HeadIdleTime)) {
                phase = 2;
                break;
            }
            if (AIAction_DoNothing() == AI_ACTION_STOPPING) {
                Terminate();
                return;
            }
            break;
        case 2:
            if (kraken->IsTargetAlive()) {
                if (!first && attacksLeft <= 0) {
                    if (AIAction_CombatPerformActionAndWait(CCreatureAction_KrakenRoarInto(GetCreature()), false) == AI_ACTION_STOPPING) {
                        Terminate();
                        return;
                    }
                    if (AIAction_CombatPerformActionAndWait(CCreatureAction_KrakenHighAttackInto(GetCreature(), *kraken->GetPTarget()), false) == AI_ACTION_STOPPING) {
                        Terminate();
                        return;
                    }
                    attacksLeft = kraken->GetKrakenDef().LowStrikesBeforeHigh;
                    startFrame = CWorld::GetFrame();
                    phase = 1;
                    startHealth = GetCreature().GetHealth();
                } else {
                    if (AIAction_CombatPerformActionAndWait(CCreatureAction_KrakenLowPoseInto(GetCreature(), *kraken->GetPTarget()), false) == AI_ACTION_STOPPING) {
                        Terminate();
                        return;
                    }
                    if (kraken->GetHeadHasBeenHit()) {
                        attacksLeft = 0;
                        kraken->SetHeadHasBeenHit(false);
                    } else {
                        startFrame = CWorld::GetFrame();
                        phase = 1;
                        startHealth = GetCreature().GetHealth();
                        --attacksLeft;
                    }
                }
                break;
            }
            if (AIAction_DoNothing() == AI_ACTION_STOPPING) {
                Terminate();
                return;
            }
            break;
        case 3:
            if (AIAction_DoNothing() == AI_ACTION_STOPPING) {
                Terminate();
                return;
            }
            break;
        }
        if (kraken->ShouldReturnHeadToWater()) {
            if (AIAction_CombatPerformActionAndWait(CCreatureAction_KrakenReturnToWater(GetCreature()), false) == AI_ACTION_STOPPING) {
                Terminate();
                return;
            }
            phase = 0;
            first = false;
            attacksLeft = 0;
        }
    }
}

// 0096ee90 behaviour test: CAIStateGroup_MeleeCombatNearby::Update (match.cpp body), host stand-ins. C++03, g++ -w.
#include <stdio.h>
#include <string>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
static std::string g_log;
static void logs(const char* s) { g_log += s; g_log += ";"; }

static int g_frame = 0;
struct CThing {
    bool Dead;
    bool CombatMode;
    int CombatantFrames;                        // IsCombatant() true while g_frame < this
    bool Movable;
    bool IsDead() const { return Dead; }
    bool IsInCombatMode() { return CombatMode; }
    bool IsCombatant() { return g_frame < CombatantFrames; }
};
static bool IsAbleToMove(CThing* thing) { return thing->Movable; }
static void RemoveMovementBlockingModes(CThing* thing) { thing->Movable = true; logs("unblock"); }

struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = item; }
};
template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(const CBaseIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};
static bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}
struct CCombatManager {
    CThing* LastCreature; CThing* LastTarget;
    void RegisterCombatant(CThing* creature, CThing* target, bool flag) { (void)flag; LastCreature = creature; LastTarget = target; logs("register"); }
};
struct CWorld { CCombatManager* PCombatManager; };
struct CAIBrain { CThing* PCreature; };

static int g_stopAt = -1, g_stopCalls = 0, g_lookResult = 2, g_moveFailAt = -1, g_runnableUntil = 1000, g_enemyDiesAt = -1;
struct CAIStateGroup_MeleeCombatNearby {
    CAIBrain* PBrain;
    CWorld* World;
    CBaseIntelligentPointer PEnemy;
    int Terminations, Attacks, Strafes;
    CThing* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPWorld() { return World; }
    bool IsStopping() { ++g_stopCalls; return g_stopAt > 0 && g_stopCalls >= g_stopAt; }
    long AIAction_LookAtThing(long repeat, CIntelligentPointer<const CThing> thing, long frames, float speed)
    { (void)repeat; (void)frames; (void)speed; logs(thing.GetPItem() == PEnemy.GetPItem() ? "look" : "look?"); return g_lookResult; }
    void Terminate() { ++Terminations; logs("T"); }
    void UpdateCurrentAction() { logs("current"); }
    bool ShouldGoStraightIntoStrafe() { return false; }
    void AddStrafeMode() { ++Strafes; logs("strafe"); }
    bool IsStillRunnable(bool flag) { (void)flag; return g_frame < g_runnableUntil; }
    void OnReadyToAttack() { logs("ready"); }
    void Combat_ProcessMeleeAttackMessage() { logs("msg"); }
    bool MoveToEnemyCreatureCombatPosition() { logs("move"); return g_frame != g_moveFailAt; }
    void AttackEnemy() { ++Attacks; logs("attack"); ++g_frame; if (g_frame == g_enemyDiesAt) PEnemy.GetPItem()->Dead = true; }
    void Update();
};

void CAIStateGroup_MeleeCombatNearby::Update()
{
    UpdateCurrentAction();
    if (IsStopping())
        goto stop;
    {
        Combat_ProcessMeleeAttackMessage();
        if (!IsAbleToMove(GetCreature()))
            RemoveMovementBlockingModes(GetCreature());
        if (IS_THING_ALIVE(PEnemy)) {
            CThing* creature = GetCreature();
            CCombatManager* combat = GetPWorld()->PCombatManager;
            combat->RegisterCombatant(creature, PEnemy.GetPItem(), true);
            if (AIAction_LookAtThing(1, CIntelligentPointer<const CThing>(PEnemy), 0x21, 0.0027777f) == 1)
                goto done;
        }
        bool strafing = GetCreature()->IsInCombatMode();
        if (ShouldGoStraightIntoStrafe()) {
            AddStrafeMode();
            strafing = false;
        }
        do {
            if (MoveToEnemyCreatureCombatPosition()) {
                OnReadyToAttack();
                if (!IsStopping()) {
                    if (strafing) {
                        AddStrafeMode();
                        strafing = false;
                    }
                    AttackEnemy();
                }
                if (!IS_THING_ALIVE(PEnemy) || IsStopping() || !IsStillRunnable(true))
                    break;
            } else {
                Terminate();
                return;
            }
        } while (GetCreature()->IsCombatant());
    }
    goto done;
stop:
    Terminate();
    return;
done:
    Terminate();
}

static void reset(CAIStateGroup_MeleeCombatNearby& g, CThing& me, CThing& enemy)
{
    g_log = ""; g_frame = 0; g_stopAt = -1; g_stopCalls = 0; g_lookResult = 2; g_moveFailAt = -1; g_runnableUntil = 1000; g_enemyDiesAt = -1;
    g.Terminations = g.Attacks = g.Strafes = 0;
    me.Dead = false; me.CombatMode = true; me.CombatantFrames = 3; me.Movable = true;
    enemy.Dead = false;
}

int main()
{
    CThing me, enemy;
    CCombatManager cm = { 0, 0 };
    CWorld world = { &cm };
    CAIBrain brain = { &me };
    CAIStateGroup_MeleeCombatNearby g;
    g.PBrain = &brain; g.World = &world; g.PEnemy.SetPItem(&enemy);

    // full fight: 3 attack rounds until no longer a combatant, strafe once (combat mode), one Terminate
    reset(g, me, enemy);
    g.Update();
    CHECK(g.Attacks == 3 && g.Strafes == 1 && g.Terminations == 1);
    CHECK(cm.LastCreature == &me && cm.LastTarget == &enemy);
    CHECK(g_log == "current;msg;register;look;move;ready;strafe;attack;move;ready;attack;move;ready;attack;T;");

    // stopping at once: only UpdateCurrentAction then Terminate
    reset(g, me, enemy); g_stopAt = 1;
    g.Update();
    CHECK(g_log == "current;T;" && g.Terminations == 1);

    // look-at result 1: terminate before the loop
    reset(g, me, enemy); g_lookResult = 1; me.Movable = false;
    g.Update();
    CHECK(g_log == "current;msg;unblock;register;look;T;" && me.Movable);

    // dead enemy: no registration, the loop still runs once and breaks
    reset(g, me, enemy); enemy.Dead = true; me.CombatMode = false;
    g.Update();
    CHECK(g_log == "current;msg;move;ready;attack;T;" && g.Strafes == 0);

    // move failure on the 2nd round terminates once
    reset(g, me, enemy); g_moveFailAt = 1;
    g.Update();
    CHECK(g.Attacks == 1 && g.Terminations == 1);
    CHECK(g_log.substr(g_log.size() - 7) == "move;T;");

    // stopping inside the loop: no attack that round, then break
    reset(g, me, enemy); g_stopAt = 2;
    g.Update();
    CHECK(g.Attacks == 0 && g.Terminations == 1 && g_log == "current;msg;register;look;move;ready;T;");

    // enemy dies after the first attack, or the group is no longer runnable
    reset(g, me, enemy); g_enemyDiesAt = 1;
    g.Update();
    CHECK(g.Attacks == 1 && g.Terminations == 1);
    reset(g, me, enemy); g_runnableUntil = 2;
    g.Update();
    CHECK(g.Attacks == 2 && g.Terminations == 1);

    if (!g_fail) printf("0096ee90_TEST PASS\n");
    return g_fail;
}
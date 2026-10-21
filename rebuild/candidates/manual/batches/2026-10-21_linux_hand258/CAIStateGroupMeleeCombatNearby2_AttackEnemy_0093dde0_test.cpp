// Behaviour test for CAIStateGroup_MeleeCombatNearby2::AttackEnemy (0x0093DDE0): model copy with scripted stubs.
#include <cstdio>

struct CThing { int id; };
struct CBrainOutput {
    unsigned long Types;
    long LookFlags;
    const CThing* LookAtThing;
    CBrainOutput() : Types(0), LookFlags(0), LookAtThing(0) { }
    void SetAsLookAtThing(const CThing* thing, long lookFlags)
    {
        Types |= 4;
        LookAtThing = thing;
        LookFlags = lookFlags;
    }
};
struct CThingCreatureBase { int combatant; bool IsRegisteredCombatant() { return combatant != 0; } };
static int g_ready, g_inRange[16], g_ir;
struct CCombatManager {
    void SetAttackerAsReadyToAttack(CThingCreatureBase*, bool ready) { if (ready) ++g_ready; }
    bool Combat_CheckRingRangeToTarget(CThingCreatureBase*) { return g_inRange[g_ir++] != 0; }
};
struct CWorld { CCombatManager* CombatManager; CCombatManager* GetPCombatManager() const { return CombatManager; } };
static int g_outputs, g_yields; static CBrainOutput g_last;
struct CAIBrain {
    CThingCreatureBase* PCreature;
    void Yield() { ++g_yields; }
    void SetBrainOutput(const CBrainOutput& o) { ++g_outputs; g_last = o; }
};
struct CPointer { CThing* item; CThing* GetPItem() const { return item; } };
static int g_stopping[32], g_is, g_terminated, g_runnable;

struct CAIStateGroup_MeleeCombatNearby2 {
    CAIBrain* PBrain; CWorld* world_; CPointer PEnemy;
    CWorld* GetPWorld() { return world_; }
    bool IsStillRunnable(bool) { return g_runnable != 0; }
    bool IsStopping() { return g_stopping[g_is++] != 0; }
    void Terminate() { ++g_terminated; }
    void AttackEnemy();
};

void CAIStateGroup_MeleeCombatNearby2::AttackEnemy()
{
    if (PBrain->PCreature->IsRegisteredCombatant()) {
        CThingCreatureBase* creature = PBrain->PCreature;
        CWorld* world = GetPWorld();
        world->CombatManager->SetAttackerAsReadyToAttack(creature, true);
    }
    while (!IsStopping()) {
        if (!IsStillRunnable(true))
            return;
        if (!PBrain->PCreature->IsRegisteredCombatant())
            return;
        if (!PBrain->PCreature->IsRegisteredCombatant())
            return;
        CThingCreatureBase* creature = PBrain->PCreature;
        CWorld* world = GetPWorld();
        if (!world->GetPCombatManager()->Combat_CheckRingRangeToTarget(creature))
            return;
        CBrainOutput output;
        output.Types |= 0x20;
        output.SetAsLookAtThing(PEnemy.GetPItem(), 0x11);
        PBrain->SetBrainOutput(output);
        PBrain->Yield();
        if (IsStopping()) {
            Terminate();
            return;
        }
    }
}

static void Reset() { g_ready = g_ir = g_outputs = g_yields = g_is = g_terminated = 0; g_runnable = 1; for (int i = 0; i < 32; ++i) g_stopping[i] = 0; for (int i = 0; i < 16; ++i) g_inRange[i] = 1; }

int main()
{
    int ok = 1;
    CThing enemy = { 9 };
    CThingCreatureBase c = { 1 };
    CCombatManager cm; CWorld w = { &cm }; CAIBrain b = { &c };
    CAIStateGroup_MeleeCombatNearby2 s; s.PBrain = &b; s.world_ = &w; s.PEnemy.item = &enemy;
    // two outputs, then out of range
    Reset(); g_inRange[2] = 0;
    s.AttackEnemy();
    if (g_ready != 1 || g_outputs != 2 || g_yields != 2 || g_terminated) ok = 0;
    if (g_last.Types != 0x24 || g_last.LookFlags != 0x11 || g_last.LookAtThing != &enemy) ok = 0;
    // stops right after the first yield: Terminate
    Reset(); g_stopping[1] = 1;
    s.AttackEnemy();
    if (g_outputs != 1 || g_terminated != 1) ok = 0;
    // already stopping: nothing posted
    Reset(); g_stopping[0] = 1; s.AttackEnemy();
    if (g_outputs != 0 || g_ready != 1) ok = 0;
    // not a combatant: no ready flag, leaves at once
    Reset(); c.combatant = 0; s.AttackEnemy();
    if (g_ready != 0 || g_outputs != 0) ok = 0;
    // not runnable
    Reset(); c.combatant = 1; g_runnable = 0; s.AttackEnemy();
    if (g_outputs != 0) ok = 0;
    if (!ok) { std::printf("0093dde0_TEST FAIL\n"); return 1; }
    std::printf("0093dde0_TEST PASS\n");
    return 0;
}

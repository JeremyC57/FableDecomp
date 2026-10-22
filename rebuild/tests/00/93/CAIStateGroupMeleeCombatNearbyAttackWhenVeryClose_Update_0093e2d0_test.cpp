// Behaviour test for CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose::Update (0x0093E2D0): model copy, scripted stubs.
#include <cstdio>

struct CThing { unsigned char m_flags91; };
struct CPtr { CThing* item; CThing* GetPItem() const { return item; } };
static bool IS_THING_ALIVE(const CPtr& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
static int g_registered, g_refreshed, g_strafes, g_ready[8], g_ri, g_outputs, g_yields, g_terminated, g_readyHooks;
static int g_stopping[64], g_si, g_runnable[64], g_rni, g_combatant[64], g_ci;
static bool g_hasPerceive, g_combatMode;
static const CThing* g_lookAt;
static long g_lookFlags, g_types;
struct CTCPerceiveThing { void Refresh() { ++g_refreshed; } };
struct CCombatManager {
    void RegisterCombatant(CThing*, CThing* target, bool flag) { if (target && flag) ++g_registered; }
    void SetAttackerAsReadyToAttack(CThing*, bool ready) { g_ready[g_ri++] = ready ? 1 : 2; }
};
static CCombatManager g_combat;
struct CCreature : CThing {
    CCombatManager* GetCombatManager() { return &g_combat; }
    bool GetInterface(CTCPerceiveThing** out, long type) { static CTCPerceiveThing p; if (type != 0x34 || !g_hasPerceive) return false; *out = &p; return true; }
    bool IsInCombatMode() { return g_combatMode; }
    bool IsCombatant() { return g_combatant[g_ci++] != 0; }
};
struct CBrainOutput {
    unsigned long Types; long LookFlags; const CThing* LookAt;
    CBrainOutput() : Types(0), LookFlags(0), LookAt(0) { }
    void SetAsLookAtThing(const CThing* t, long f) { Types |= 4; LookAt = t; LookFlags = f; }
};
struct CWorld { CCombatManager* PCombatManager; };
static CWorld g_world = { &g_combat };
struct CAIBrain {
    CCreature* PCreature;
    void SetBrainOutput(const CBrainOutput& o) { ++g_outputs; g_lookAt = o.LookAt; g_lookFlags = o.LookFlags; g_types = (long)o.Types; }
    void Yield() { ++g_yields; }
};
struct CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose {
    CAIBrain* PBrain;
    CPtr PEnemy;
    CCreature* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPWorld() { return &g_world; }
    bool IsStopping() { return g_stopping[g_si++] != 0; }
    bool IsStillRunnable(bool f) { return f && g_runnable[g_rni++] != 0; }
    void OnReadyToAttack() { ++g_readyHooks; }
    void AddStrafeMode() { ++g_strafes; }
    void Terminate() { ++g_terminated; }
    void Update();
};

void CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose::Update()
{
    if (!IS_THING_ALIVE(PEnemy)) {
        Terminate();
        return;
    }
    CCreature* creature = GetCreature();
    creature->GetCombatManager()->RegisterCombatant(creature, PEnemy.GetPItem(), true);
    CTCPerceiveThing* perceive;
    if (GetCreature()->GetInterface(&perceive, 0x34))
        perceive->Refresh();
    if (GetCreature()->IsInCombatMode())
        AddStrafeMode();
    OnReadyToAttack();
    if (GetCreature()->IsCombatant()) {
        CThing* attacker = GetCreature();
        CWorld* world = GetPWorld();
        world->PCombatManager->SetAttackerAsReadyToAttack(attacker, true);
    }
    while (IS_THING_ALIVE(PEnemy) && !IsStopping() && IsStillRunnable(true) && GetCreature()->IsCombatant()) {
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
    if (GetCreature()->IsCombatant()) {
        CThing* attacker = GetCreature();
        CWorld* world = GetPWorld();
        world->PCombatManager->SetAttackerAsReadyToAttack(attacker, false);
    }
    Terminate();
}

static void Reset()
{
    g_registered = g_refreshed = g_strafes = g_ri = g_outputs = g_yields = g_terminated = g_readyHooks = 0;
    g_si = g_rni = g_ci = 0; g_ready[0] = g_ready[1] = 0;
    for (int i = 0; i < 64; ++i) { g_stopping[i] = 0; g_runnable[i] = 1; g_combatant[i] = 1; }
}

int main()
{
    int ok = 1;
    CThing enemy = { 0 };
    CCreature me; me.m_flags91 = 0;
    CAIBrain brain = { &me };
    CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose s; s.PBrain = &brain; s.PEnemy.item = 0;
    // no enemy: terminate only
    Reset(); s.Update(); if (g_terminated != 1 || g_registered || g_readyHooks) ok = 0;
    s.PEnemy.item = &enemy;
    // dead enemy
    enemy.m_flags91 = 1; Reset(); s.Update(); if (g_terminated != 1 || g_registered) ok = 0;
    enemy.m_flags91 = 0;
    // three looks then IsStillRunnable fails; ready true at start, false at the end
    g_hasPerceive = true; g_combatMode = true;
    Reset(); g_runnable[3] = 0; s.Update();
    if (g_registered != 1 || g_refreshed != 1 || g_strafes != 1 || g_readyHooks != 1) ok = 0;
    if (g_outputs != 3 || g_yields != 3 || g_lookAt != &enemy || g_lookFlags != 0x11 || g_types != 0x24) ok = 0;
    if (g_ri != 2 || g_ready[0] != 1 || g_ready[1] != 2 || g_terminated != 1) ok = 0;
    // stop flag after the first yield: terminate inside the loop, no final ready mark
    g_hasPerceive = false; g_combatMode = false;
    Reset(); g_stopping[1] = 1; s.Update();
    if (g_refreshed || g_strafes || g_outputs != 1 || g_terminated != 1 || g_ri != 1) ok = 0;
    // not a combatant at the start: no ready marks at all, no loop
    Reset(); for (int i = 0; i < 64; ++i) g_combatant[i] = 0; s.Update();
    if (g_ri != 0 || g_outputs || g_terminated != 1) ok = 0;
    if (!ok) { std::printf("0093e2d0_TEST FAIL\n"); return 1; }
    std::printf("0093e2d0_TEST PASS\n");
    return 0;
}
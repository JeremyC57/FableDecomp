#include <cstdio>

struct CThing { bool alive; };
struct CCombatManager {
    CThing* attacker; CThing* target; int registers, deregisters;
    bool IsCreatureAttacking(const CThing& t) const { return attacker == &t; }
    void DeregisterCombatant(CThing* t) { if (attacker == t) attacker = 0; ++deregisters; }
    void RegisterCombatant(CThing* t, CThing* victim, bool) { attacker = t; target = victim; ++registers; }
};
struct CTCBoss { long next; void SetNextBossPhase(long p, bool) { next = p; } };
struct CTCKraken {
    CCombatManager* combat; CThing* ParentThing; CThing* PTarget; CTCBoss* PTCBoss; long Phase; long Mode;
    int tentacles; int created, raised, ended;
    void CreateTentacles() { ++created; tentacles = 4; }
    void RaiseSunkenTentacles() { ++raised; }
    void EndHeadEffects() { ++ended; }
    void InitTentaclesAndHead() { if (tentacles == 0) CreateTentacles(); RaiseSunkenTentacles(); EndHeadEffects(); }
    void SetPhase(long phase)
    {
        CCombatManager* c = combat;
        CThing* thing = ParentThing;
        Phase = phase;
        PTCBoss->SetNextBossPhase(phase, false);
        switch (Phase) {
        case 0:
        case 3:
            if (c->IsCreatureAttacking(*thing))
                c->DeregisterCombatant(thing);
            break;
        case 1:
        case 2:
            if (!c->IsCreatureAttacking(*thing)) {
                if (PTarget && PTarget->alive)
                    c->RegisterCombatant(thing, PTarget, false);
            }
            Mode = 1;
            InitTentaclesAndHead();
            break;
        }
    }
};
static bool Run()
{
    CThing me, hero; me.alive = true; hero.alive = true;
    CCombatManager cm; cm.attacker = 0; cm.target = 0; cm.registers = 0; cm.deregisters = 0;
    CTCBoss boss; boss.next = -1;
    CTCKraken k; k.combat = &cm; k.ParentThing = &me; k.PTarget = &hero; k.PTCBoss = &boss; k.Phase = 0; k.Mode = 0;
    k.tentacles = 0; k.created = 0; k.raised = 0; k.ended = 0;
    k.SetPhase(1);
    bool ok = k.Phase == 1 && boss.next == 1 && cm.attacker == &me && cm.target == &hero && k.Mode == 1 && k.created == 1 && k.raised == 1;
    k.SetPhase(2);
    ok = ok && cm.registers == 1 && k.created == 1 && k.raised == 2 && k.ended == 2;
    k.SetPhase(3);
    ok = ok && cm.attacker == 0 && cm.deregisters == 1 && boss.next == 3;
    k.SetPhase(0); ok = ok && cm.deregisters == 1;
    hero.alive = false; k.Mode = 0; k.SetPhase(2);
    ok = ok && cm.registers == 1 && k.Mode == 1;
    k.SetPhase(5); return ok && k.Phase == 5 && boss.next == 5 && k.raised == 3;
}

int main()
{
    if (!Run()) { std::printf("007fc820_TEST FAIL\n"); return 1; }
    std::printf("007fc820_TEST PASS\n");
    return 0;
}

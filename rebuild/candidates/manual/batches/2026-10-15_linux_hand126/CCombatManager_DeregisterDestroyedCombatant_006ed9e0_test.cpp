// Behaviour test for 006ed9e0 CCombatManager::DeregisterDestroyedCombatant: an attacking creature is
// deregistered as attacker and its target's stealth component (when present) is told the danger is
// over; a combatant is deregistered as attacked; the target is looked up before the attacker entry goes.
#include <cstdio>
#include <map>

struct CThing;
struct CTCStealth { CThing* lastSource; int calls; void SetAsInDanger(CThing* s, int status) { lastSource = s; calls += (status == 0); } };
struct CThing { bool hasStealth; CTCStealth stealth; bool attacking, combatant; long id;
    bool GetInterface(CTCStealth** out) { if (!hasStealth) return false; *out = &stealth; return true; } };
struct CManager {
    std::map<long, CThing*> attackedBy;                     // attacker id -> its target
    int attackerDeregs, attackedDeregs;
    void DeregisterAttackingCombatant(CThing* c) { ++attackerDeregs; attackedBy.erase(c->id); }
    void DeregisterAttackedCombatant(CThing*) { ++attackedDeregs; }
    void DeregisterDestroyedCombatant(CThing* creature)
    {
        if (creature->attacking) {
            CThing* attacked = attackedBy.find(creature->id)->second;
            DeregisterAttackingCombatant(creature);
            CTCStealth* stealth;
            if (attacked->GetInterface(&stealth)) stealth->SetAsInDanger(creature, 0);
        }
        if (creature->combatant) DeregisterAttackedCombatant(creature);
    }
};

int main()
{
    CThing hero; hero.hasStealth = true; hero.stealth.calls = 0; hero.stealth.lastSource = 0; hero.attacking = false; hero.combatant = true; hero.id = 1;
    CThing bandit; bandit.hasStealth = false; bandit.attacking = true; bandit.combatant = true; bandit.id = 2;
    CManager m; m.attackerDeregs = 0; m.attackedDeregs = 0; m.attackedBy[2] = &hero;
    m.DeregisterDestroyedCombatant(&bandit);
    bool ok = m.attackerDeregs == 1 && m.attackedDeregs == 1 && hero.stealth.calls == 1 && hero.stealth.lastSource == &bandit && m.attackedBy.empty();
    m.DeregisterDestroyedCombatant(&hero);
    ok = ok && m.attackerDeregs == 1 && m.attackedDeregs == 2;
    if (!ok) { std::printf("006ed9e0_TEST FAIL\n"); return 1; }
    std::printf("006ed9e0_TEST PASS\n");
    return 0;
}

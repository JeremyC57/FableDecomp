// Behaviour test for CCombatManager::RegisterAttackedCombatant (0x006ED270): a missing combatant gets a record
// (Parent set, no target), the attacker joins its attackers, the pattern is reset with the continuous pattern def
// and the creature is flagged as attacked.
#include <cstdio>
#include <cstring>

struct CThingCreatureBase { unsigned long UID; bool IsAttackedCombatant; unsigned long GetUID() const { return UID; } };
struct CAttackPatternDef { long m_refCount; };
struct CCombatantInfo {
    CThingCreatureBase* Parent; CThingCreatureBase* Attacked;
    CThingCreatureBase* Attackers[4]; int AttackerCount; CAttackPatternDef* Pattern;
    CCombatantInfo() : Parent(0), Attacked((CThingCreatureBase*)1), AttackerCount(0), Pattern(0) {}
};
struct CCombatManager {
    unsigned long Keys[4]; CCombatantInfo Infos[4]; int Count;
    CAttackPatternDef Continuous;
    CCombatantInfo* find(unsigned long k) { for (int i = 0; i < Count; ++i) if (Keys[i] == k) return &Infos[i]; return 0; }
    CCombatantInfo* insert(unsigned long k, const CCombatantInfo& v) { Keys[Count] = k; Infos[Count] = v; return &Infos[Count++]; }
    void ResetDefenderAttackPattern(CCombatantInfo& info, CAttackPatternDef* p) { info.Pattern = p; }
    void RegisterAttackedCombatant(CThingCreatureBase& attacker, CThingCreatureBase& attacked)
    {
        CCombatantInfo* it = find(attacked.GetUID());
        if (it == 0) {
            CCombatantInfo info;
            info.Parent = &attacked;
            info.Attacked = 0;
            it = insert(attacked.GetUID(), info);
        }
        CCombatantInfo& info = *it;
        bool known = false;
        for (int i = 0; i < info.AttackerCount; ++i) known = known || info.Attackers[i] == &attacker;
        if (!known) info.Attackers[info.AttackerCount++] = &attacker;   // map insert: unique keys
        ++Continuous.m_refCount;
        ResetDefenderAttackPattern(info, &Continuous);
        --Continuous.m_refCount;
        attacked.IsAttackedCombatant = true;
    }
};

int main()
{
    CCombatManager m; m.Count = 0; m.Continuous.m_refCount = 1;
    CThingCreatureBase hero = { 10, false }, bandit = { 20, false }, wolf = { 30, false };
    m.RegisterAttackedCombatant(bandit, hero);
    m.RegisterAttackedCombatant(wolf, hero);
    m.RegisterAttackedCombatant(wolf, hero);
    bool ok = m.Count == 1 && m.Keys[0] == 10 && m.Infos[0].Parent == &hero && m.Infos[0].Attacked == 0
        && m.Infos[0].AttackerCount == 2 && m.Infos[0].Pattern == &m.Continuous && m.Continuous.m_refCount == 1
        && hero.IsAttackedCombatant && !bandit.IsAttackedCombatant;
    if (!ok) { std::printf("006ed270_TEST FAIL\n"); return 1; }
    std::printf("006ed270_TEST PASS\n");
    return 0;
}
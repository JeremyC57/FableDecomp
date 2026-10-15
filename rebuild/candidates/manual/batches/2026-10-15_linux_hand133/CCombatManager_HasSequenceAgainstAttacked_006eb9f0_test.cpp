// Behaviour test for CCombatManager::HasSequenceAgainstAttacked @ 0x006EB9F0
#include <cstdio>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
struct CThing { u64 UniqueID; u64 GetUID() const { return UniqueID; } };
struct CAttackingCombatantInfo { CThing* Attacked; };
struct CCombatantInfo { CThing* Parent; CAttackingCombatantInfo AttackerInfo; long bestSequence; };
struct CMap {
    struct node { u64 key; CCombatantInfo value; };
    node* nodes; int count;
    node* find(const u64& key) { for (int i = 0; i < count; ++i) if (nodes[i].key == key) return &nodes[i]; return 0; }
};
struct CCombatManager {
    CMap Combatants;
    CCombatantInfo& GetCombatantInfo(u64 uid) { return Combatants.find(uid)->value; }
    long GetBestSequenceIndexAttackerOnDefender(CCombatantInfo& attacker, CCombatantInfo& defender)
    { return attacker.AttackerInfo.Attacked == defender.Parent ? attacker.bestSequence : -2; }
    bool HasSequenceAgainstAttacked(const CThing& creature)
    {
        CCombatantInfo& attacker = GetCombatantInfo(creature.GetUID());
        CCombatantInfo& defender = GetCombatantInfo(attacker.AttackerInfo.Attacked->GetUID());
        return GetBestSequenceIndexAttackerOnDefender(attacker, defender) >= 0 ? true : false;
    }
};
int main()
{
    CThing hero, bandit;
    hero.UniqueID = ((u64)1 << 32) | 1; bandit.UniqueID = ((u64)2 << 32) | 2;
    CMap::node nodes[2];
    nodes[0].key = hero.UniqueID; nodes[0].value.Parent = &hero; nodes[0].value.AttackerInfo.Attacked = &bandit; nodes[0].value.bestSequence = 3;
    nodes[1].key = bandit.UniqueID; nodes[1].value.Parent = &bandit; nodes[1].value.AttackerInfo.Attacked = &hero; nodes[1].value.bestSequence = -1;
    CCombatManager m; m.Combatants.nodes = nodes; m.Combatants.count = 2;
    if (!m.HasSequenceAgainstAttacked(hero) || m.HasSequenceAgainstAttacked(bandit)) { std::printf("006eb9f0_TEST FAIL\n"); return 1; }
    nodes[0].value.bestSequence = 0;
    if (!m.HasSequenceAgainstAttacked(hero)) { std::printf("006eb9f0_TEST FAIL\n"); return 1; }
    std::printf("006eb9f0_TEST PASS\n");
    return 0;
}

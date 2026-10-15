#include <cstdio>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
// CCombatManager::IsCreatureBeingAttacked_006e9270: find the creature's UniqueID in the combatant map; true when found and (it->value.Attackers.size() != 0)
struct CThingCreatureBase { long pad[6]; u64 UniqueID; u64 GetUID() const { return UniqueID; } };
struct CStringSetLike { void* header; unsigned long count; unsigned long comp; unsigned long size() const { return count; } };
struct CCombatantInfo { CThingCreatureBase* Parent; CThingCreatureBase* Attacked; unsigned char rest[0x2c]; CStringSetLike Attackers; };
struct CMap {
    struct node { long colour; node* parent; node* left; node* right; u64 key; CCombatantInfo value; };
    node* header; node* table; long n;
    node* find(const u64& key) { for (long i = 0; i < n; ++i) if (table[i].key == key) return &table[i]; return header; }
    node* end() { return header; }
};
struct CCombatManager {
    void* vtable; unsigned char seq[0xc]; CMap Combatants;
    bool IsCreatureBeingAttacked_006e9270(const CThingCreatureBase& creature)
    {
        CMap::node* it = Combatants.find(creature.GetUID());
        if (it != Combatants.end() && it->value.Attackers.size() != 0)
            return true;
        return false;
    }
};
int main()
{
    CThingCreatureBase creatures[3];
    creatures[0].UniqueID = (((u64)1) << 32) | 1; creatures[1].UniqueID = (((u64)2) << 32) | 2; creatures[2].UniqueID = (((u64)3) << 32) | 3;
    CMap::node head; CMap::node nodes[2];
    nodes[0].key = creatures[0].UniqueID; nodes[1].key = creatures[1].UniqueID;
    nodes[0].value.Attackers.count = 2; nodes[1].value.Attackers.count = 0;
    CCombatManager m; m.Combatants.header = &head; m.Combatants.table = nodes; m.Combatants.n = 2;
    bool ok = m.IsCreatureBeingAttacked_006e9270(creatures[0]) && !m.IsCreatureBeingAttacked_006e9270(creatures[1]) && !m.IsCreatureBeingAttacked_006e9270(creatures[2]);
    if (!ok) { std::printf("006e9270_TEST FAIL\n"); return 1; }
    std::printf("006e9270_TEST PASS\n");
    return 0;
}

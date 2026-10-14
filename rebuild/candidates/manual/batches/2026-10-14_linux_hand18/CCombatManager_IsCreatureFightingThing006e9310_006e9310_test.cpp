#include <cstdio>
struct CThing_Calls { int x; };
struct CThingCreatureBase_Retail { unsigned char _pad[0x18]; unsigned __int64 UniqueID; unsigned __int64 GetUID() const { return UniqueID; } };
struct CCombatantInfo_Retail { long Mode; CThing_Calls* PTarget; };
struct CRetailRbMap_UID_CombatantInfo {
    struct node { long colour; node* parent; node* left; node* right; unsigned __int64 key; CCombatantInfo_Retail value; };
    node* header; unsigned long count; unsigned long comp;
    node* find(const unsigned __int64& key) { for (unsigned long i = 0; i < count; ++i) if (header[1 + i].key == key) return &header[1 + i]; return header; }
    node* end() { return header; }
};
struct CCombatManager_Retail {
    virtual void v00() { } unsigned char _sequences[0xc]; CRetailRbMap_UID_CombatantInfo Combatants;
    bool IsCreatureFightingThing_006e9310(const CThingCreatureBase_Retail& creature, const CThing_Calls* pThing)
    {
        CRetailRbMap_UID_CombatantInfo::node* it = Combatants.find(creature.GetUID());
        if (it != Combatants.end() && pThing == it->value.PTarget) return true;
        return false;
    }
};
int main()
{
    CThing_Calls t1, t2; CRetailRbMap_UID_CombatantInfo::node nodes[3];
    nodes[1].key = 0x1000000020ULL; nodes[1].value.PTarget = &t1; nodes[2].key = 7; nodes[2].value.PTarget = &t2;
    CCombatManager_Retail m; m.Combatants.header = nodes; m.Combatants.count = 2;
    CThingCreatureBase_Retail c; c.UniqueID = 0x1000000020ULL;
    if (!m.IsCreatureFightingThing_006e9310(c, &t1) || m.IsCreatureFightingThing_006e9310(c, &t2)) { std::printf("006e9310_TEST FAIL\n"); return 1; }
    c.UniqueID = 7; if (!m.IsCreatureFightingThing_006e9310(c, &t2)) { std::printf("006e9310_TEST FAIL\n"); return 1; }
    c.UniqueID = 8; if (m.IsCreatureFightingThing_006e9310(c, &t2) || m.IsCreatureFightingThing_006e9310(c, 0)) { std::printf("006e9310_TEST FAIL\n"); return 1; }
    if (sizeof(CRetailRbMap_UID_CombatantInfo::node) != 0x20 || (char*)&nodes[0].value.PTarget - (char*)&nodes[0] != 0x1c) { std::printf("006e9310_TEST FAIL\n"); return 1; }
    std::printf("006e9310_TEST PASS\n"); return 0;
}

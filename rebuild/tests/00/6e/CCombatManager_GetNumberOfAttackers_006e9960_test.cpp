#include <cstdio>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
class CThingCreatureBase { public: u64 UniqueID; bool Combatant; bool IsCombatant() { return Combatant; } };
struct CAttackerMap { void* Header; unsigned long Count; };
struct CCombatantInfo { CAttackerMap Attackers; };
struct SCombatantNode { u64 Key; CCombatantInfo Value; };
static SCombatantNode g_nodes[2];
static SCombatantNode g_header;
static u64 g_lastKey = 0;
struct CCombatantMap {
    SCombatantNode* FindNode(const u64& key)
    {
        g_lastKey = key;
        for (int i = 0; i < 2; ++i) if (g_nodes[i].Key == key) return &g_nodes[i];
        return &g_header;
    }
};
class CCombatManager { public:
    CCombatantMap Combatants;
    long GetNumberOfAttackers(CThingCreatureBase* creature)
    {
        if (creature->IsCombatant()) {
            u64 id = creature->UniqueID;
            return Combatants.FindNode(id)->Value.Attackers.Count;
        }
        return 0;
    }
};
int main()
{
    g_nodes[0].Key = 0x100000001ULL; g_nodes[0].Value.Attackers.Count = 3;
    g_nodes[1].Key = 7; g_nodes[1].Value.Attackers.Count = 1;
    CCombatManager m;
    CThingCreatureBase c1 = { 0x100000001ULL, true }, c2 = { 7, false };
    if (m.GetNumberOfAttackers(&c1) != 3 || g_lastKey != 0x100000001ULL || m.GetNumberOfAttackers(&c2) != 0 || g_lastKey != 0x100000001ULL)
    { std::printf("006e9960_TEST FAIL\n"); return 1; }
    std::printf("006e9960_TEST PASS\n");
    return 0;
}
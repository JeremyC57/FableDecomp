#include <cstdio>
// ---- test model: a thing's interface bitset + sorted {type, component} table, intelligent pointers ----
struct CThing;
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags91;
    CInterfaceMap::SEntry m_table[8];
    int m_count;
    void Init() { for (int i = 0; i < 9; ++i) m_presentBits[i] = 0; m_flags91 = 0; m_count = 0; Sync(); }
    void Sync() { m_interfaces.first = m_table; m_interfaces.last = m_table + m_count; }
    void Add(long type, void* comp)       // keep the table sorted
    {
        int i = m_count;
        while (i > 0 && m_table[i - 1].type > type) { m_table[i] = m_table[i - 1]; --i; }
        m_table[i].type = type; m_table[i].component = comp; ++m_count;
        m_presentBits[type >> 5] |= 1UL << (type & 31);
        Sync();
    }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* p) { m_pItem = p; }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CAIBrain { CThing* PCreature; };

static int g_pumps = 0, g_terminated = 0, g_readyFlags = 0, g_readyAfter = 3, g_result = 2;
struct CThingCreatureBase { bool registered; };
struct CCombatManager {
    void SetAttackerAsReadyToAttack(CThingCreatureBase*, bool) { ++g_readyFlags; }
    bool IsAttackerReadyToAttack(CThingCreatureBase*) { return g_pumps >= g_readyAfter; }
};
struct CAIStateGroup_MeleeCombatNearby {
    CBaseIntelligentPointer PEnemy;
    CThingCreatureBase* m_creature;
    CCombatManager* m_manager;
    bool IsStopping() { return false; }
    bool IsStillRunnable(bool) { return true; }
    bool IsEnemyCreatureNearCombatPosition() { return true; }
    void Terminate() { ++g_terminated; }
    int AIAction_PumpCombatActions() { ++g_pumps; return g_result; }
    CCombatManager* GetPCombatManager() { return m_manager; }
    void AttackEnemy()
    {
        if (m_creature->registered)
            m_manager->SetAttackerAsReadyToAttack(m_creature, true);
        do {
            int result = AIAction_PumpCombatActions();
            if (!IS_THING_ALIVE(PEnemy) || (result != 2 && result != 4))
                Terminate();
            if (IsStopping()) return;
            if (!IsStillRunnable(true)) return;
            if (!m_creature->registered) return;
            if (!IsEnemyCreatureNearCombatPosition()) return;
            if (GetPCombatManager()->IsAttackerReadyToAttack(m_creature)) return;
        } while (true);
    }
};

int main()
{
    CThing enemy; enemy.Init();
    CThingCreatureBase creature = { true };
    CCombatManager manager;
    CAIStateGroup_MeleeCombatNearby g; g.PEnemy.SetPItem(&enemy); g.m_creature = &creature; g.m_manager = &manager;
    g.AttackEnemy();
    bool ok = g_pumps == 3 && g_terminated == 0 && g_readyFlags == 1;
    g_pumps = 0; g_result = 1; g_readyAfter = 1;
    g.AttackEnemy();
    ok = ok && g_pumps == 1 && g_terminated == 1;
    if (!ok) { std::printf("0096ea50_TEST FAIL\n"); return 1; }
    std::printf("0096ea50_TEST PASS\n");
    return 0;
}

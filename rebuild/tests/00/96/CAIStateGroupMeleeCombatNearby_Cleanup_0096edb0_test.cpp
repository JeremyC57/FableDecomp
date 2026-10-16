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

static int g_attackQueries = 0, g_waitQueries = 0, g_deregistered = 0;
static unsigned long g_removedMode = 0;
struct CThingCreatureBase { bool registered; bool attacking; bool combatMode; };
struct CTCCreatureModeManager { void RemoveMode(unsigned long m) { g_removedMode = m; } };

struct CAIStateGroup_MeleeCombatNearby {
    CAIBrain* PBrain;
    CThingCreatureBase* m_creature;
    bool IsCreatureAttacking() { ++g_attackQueries; return m_creature->attacking; }
    bool IsCreatureWaitingToAttack() { ++g_waitQueries; return m_creature->attacking; }
    void Cleanup()
    {
        if (!m_creature->registered || IsCreatureAttacking()) {
            if (!m_creature->registered)
                IsCreatureWaitingToAttack();
        }
        if (m_creature->registered) {
            ++g_deregistered; m_creature->registered = false;
        }
        if (m_creature->combatMode) {
            CTCCreatureModeManager* modes;
            PBrain->PCreature->GetInterface(&modes, 0x31);
            modes->RemoveMode(0x19);
        }
    }
};

int main()
{
    CThing thing; thing.Init();
    CTCCreatureModeManager modes; thing.Add(0x31, &modes);
    CAIBrain brain = { &thing };
    CThingCreatureBase c = { true, false, true };
    CAIStateGroup_MeleeCombatNearby g; g.PBrain = &brain; g.m_creature = &c;
    g.Cleanup();
    bool ok = g_attackQueries == 1 && g_waitQueries == 0 && g_deregistered == 1 && g_removedMode == 0x19 && !c.registered;
    g_removedMode = 0; c.combatMode = false;
    g.Cleanup();
    ok = ok && g_attackQueries == 1 && g_waitQueries == 1 && g_deregistered == 1 && g_removedMode == 0;
    if (!ok) { std::printf("0096edb0_TEST FAIL\n"); return 1; }
    std::printf("0096edb0_TEST PASS\n");
    return 0;
}
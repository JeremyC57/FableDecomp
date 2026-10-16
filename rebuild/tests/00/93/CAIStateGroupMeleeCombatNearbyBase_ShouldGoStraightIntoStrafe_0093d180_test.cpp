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

struct C3DVector { float x, y, z; };
struct CThingCreatureBase { C3DVector pos; bool combat; const C3DVector& GetPosition() { return pos; } };
static C3DVector g_preferred;

struct CAIStateGroup_MeleeCombatNearbyBase {
    CThingCreatureBase* m_creature;
    CBaseIntelligentPointer PEnemy;
    CThingCreatureBase* m_enemy;
    bool m_stopping;
    bool ShouldGoStraightIntoStrafe()
    {
        if (m_creature->combat && IS_THING_ALIVE(PEnemy) && !m_stopping) {
            C3DVector preferred = g_preferred;
            const C3DVector& enemyPos = m_enemy->GetPosition();
            const C3DVector& creaturePos = m_creature->GetPosition();
            float cx = creaturePos.x - enemyPos.x, cy = creaturePos.y - enemyPos.y;
            float px = preferred.x - enemyPos.x, py = preferred.y - enemyPos.y;
            if (cx * cx + cy * cy < px * px + py * py)
                return true;
        }
        return false;
    }
};

int main()
{
    CThing enemyThing; enemyThing.Init();
    CThingCreatureBase creature = { { 1.0f, 0.0f, 0.0f }, true };
    CThingCreatureBase enemy = { { 0.0f, 0.0f, 5.0f }, true };
    g_preferred.x = 2.0f; g_preferred.y = 0.0f; g_preferred.z = 0.0f;
    CAIStateGroup_MeleeCombatNearbyBase g; g.m_creature = &creature; g.m_enemy = &enemy; g.m_stopping = false;
    g.PEnemy.SetPItem(&enemyThing);
    bool ok = g.ShouldGoStraightIntoStrafe();                  // 1 < 2: already closer than the preferred spot
    creature.pos.x = 3.0f; ok = ok && !g.ShouldGoStraightIntoStrafe();
    creature.pos.x = 1.0f; g.m_stopping = true; ok = ok && !g.ShouldGoStraightIntoStrafe();
    g.m_stopping = false; enemyThing.m_flags91 = 1; ok = ok && !g.ShouldGoStraightIntoStrafe();
    enemyThing.m_flags91 = 0; creature.combat = false; ok = ok && !g.ShouldGoStraightIntoStrafe();
    if (!ok) { std::printf("0093d180_TEST FAIL\n"); return 1; }
    std::printf("0093d180_TEST PASS\n");
    return 0;
}
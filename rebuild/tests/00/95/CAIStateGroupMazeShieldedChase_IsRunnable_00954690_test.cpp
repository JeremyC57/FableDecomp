#include <cstdio>
// ---- test model: a thing's interface bitset + sorted {type, component} table, intelligent pointers ----
class CThing;
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const
    {
        SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};
class CThing { public:
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
class CAIBrain { public: CThing* PCreature; };

struct CTCCombatTarget { CThing* enemy; CThing* GetEnemy() { return enemy; } };
struct CAIStateGroup_MazeShieldedChase {
    CAIBrain* PBrain;
    CBaseIntelligentPointer PTarget;
    bool IsRunnable()
    {
        CThing* c = PBrain->PCreature;
        if (c->m_presentBits[0x93 >> 5] & (1UL << (0x93 & 31))) {
            CTCCombatTarget* combat;
            PBrain->PCreature->GetInterface(&combat, 8);
            CThing* enemy = combat->GetEnemy();
            if (enemy && !(enemy->m_flags91 & 1)) {
                PTarget.SetPItem(enemy);
                return true;
            }
        }
        return false;
    }
};

int main()
{
    CThing c, enemy; c.Init(); enemy.Init();
    CTCCombatTarget combat = { &enemy };
    c.Add(8, &combat);
    CAIBrain brain = { &c };
    CAIStateGroup_MazeShieldedChase g; g.PBrain = &brain; g.PTarget.SetPItem(0);
    bool ok = !g.IsRunnable();                  // no 0x93 interface
    c.Add(0x93, 0);
    ok = ok && g.IsRunnable() && g.PTarget.GetPItem() == &enemy;
    g.PTarget.SetPItem(0); enemy.m_flags91 = 1; ok = ok && !g.IsRunnable() && g.PTarget.GetPItem() == 0;
    combat.enemy = 0; ok = ok && !g.IsRunnable();
    if (!ok) { std::printf("00954690_TEST FAIL\n"); return 1; }
    std::printf("00954690_TEST PASS\n");
    return 0;
}
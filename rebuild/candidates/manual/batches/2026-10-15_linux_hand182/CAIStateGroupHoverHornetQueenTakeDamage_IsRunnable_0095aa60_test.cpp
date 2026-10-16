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

struct CTCBoss { long phase; long GetBossPhase() { return phase; } };
static bool g_hit = true;
struct CAIStateGroup_HoverHornetQueenTakeDamage {
    CAIBrain* PBrain;
    CBaseIntelligentPointer PTarget;
    CThing* m_player;
    unsigned char m_queenFlags;
    bool FindHitEvent() { return g_hit; }
    bool IsRunnable()
    {
        PTarget.SetPItem(m_player);
        CTCBoss* boss;
        PBrain->PCreature->GetInterface(&boss, 0xad);
        long phase = boss->GetBossPhase();
        if ((phase == 3 || phase == 6) && FindHitEvent() && !(m_queenFlags & 0x40))
            return true;
        return false;
    }
};

int main()
{
    CThing queen, player; queen.Init(); player.Init();
    CTCBoss boss = { 1 };
    queen.Add(0xad, &boss);
    CAIBrain brain = { &queen };
    CAIStateGroup_HoverHornetQueenTakeDamage g; g.PBrain = &brain; g.m_player = &player; g.m_queenFlags = 0;
    bool ok = !g.IsRunnable() && g.PTarget.GetPItem() == &player;
    boss.phase = 3; ok = ok && g.IsRunnable();
    boss.phase = 6; ok = ok && g.IsRunnable();
    g_hit = false; ok = ok && !g.IsRunnable();
    g_hit = true; g.m_queenFlags = 0x40; ok = ok && !g.IsRunnable();
    if (!ok) { std::printf("0095aa60_TEST FAIL\n"); return 1; }
    std::printf("0095aa60_TEST PASS\n");
    return 0;
}

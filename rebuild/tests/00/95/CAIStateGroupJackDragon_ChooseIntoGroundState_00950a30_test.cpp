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

static unsigned int GFROR13(unsigned int v) { return (v >> 13) | (v << 19); }
inline unsigned int WorldRandomRange(unsigned int& seed, unsigned int range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range == 0)
        return 0;
    return seed % range;
}
static int g_released = 0;
struct CJackDragonDef { long refs; float ChanceIntoGround; };
struct CDefPointer {
    CJackDragonDef* m_p;
    ~CDefPointer() { if (m_p && --m_p->refs == 0) ++g_released; }
};
static CJackDragonDef g_def = { 0, 50.0f };
struct CDefClassBase { bool GetSubDef(CDefPointer& out) { out.m_p = &g_def; ++g_def.refs; return true; } };
struct CTCBoss { long phase; long GetBossPhase() { return phase; } };

struct CAIStateGroup_JackDragon {
    CAIBrain* PBrain;
    CDefClassBase* m_creatureDef;
    unsigned int m_seed;
    long ChooseIntoGroundState()
    {
        CDefPointer def;
        def.m_p = 0;
        m_creatureDef->GetSubDef(def);
        CTCBoss* boss;
        PBrain->PCreature->GetInterface(&boss, 0xad);
        if (boss->GetBossPhase() >= 2) {
            if ((float)WorldRandomRange(m_seed, 100) < def.m_p->ChanceIntoGround)
                return 5;
        }
        return 0;
    }
};

int main()
{
    CThing dragon; dragon.Init();
    CTCBoss boss = { 1 };
    dragon.Add(0xad, &boss);
    CAIBrain brain = { &dragon };
    CDefClassBase cdef;
    CAIStateGroup_JackDragon d; d.PBrain = &brain; d.m_creatureDef = &cdef; d.m_seed = 99;
    bool ok = d.ChooseIntoGroundState() == 0 && g_released == 1 && d.m_seed == 99;
    boss.phase = 2;
    int dives = 0;
    for (int i = 0; i < 200; ++i) {
        unsigned int s = d.m_seed; unsigned int expect = WorldRandomRange(s, 100);
        long r = d.ChooseIntoGroundState();
        if (r != ((float)expect < 50.0f ? 5 : 0)) ok = false;
        if (r == 5) ++dives;
    }
    ok = ok && dives > 0 && dives < 200 && g_released == 201 && g_def.refs == 0;
    if (!ok) { std::printf("00950a30_TEST FAIL\n"); return 1; }
    std::printf("00950a30_TEST PASS\n");
    return 0;
}
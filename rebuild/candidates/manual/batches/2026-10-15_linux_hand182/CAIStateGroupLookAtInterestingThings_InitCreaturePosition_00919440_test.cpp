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
struct CTCVillage { bool curfew; bool IsCurfewTime() { return curfew; } };
struct CTCVillager { CTCVillage* village; };
CTCVillage* GFGetPVillageInterface(CTCVillager* v) { return v->village; }
static int g_placed = 0;

struct CAIStateGroup_LookAtInterestingThings {
    CAIBrain* PBrain;
    unsigned int m_seed;
    CThing* m_thing;
    unsigned int* GetPWorldSeed() { return &m_seed; }
    CThing* FindInterestingThing() { return m_thing; }
    bool PlaceNearInterestingThing() { ++g_placed; return true; }
    bool InitCreaturePosition()
    {
        CTCVillager* villager;
        if (PBrain->PCreature->GetInterface(&villager, 0x23)
            && GFGetPVillageInterface(villager) != 0
            && !GFGetPVillageInterface(villager)->IsCurfewTime())
            return false;
        return FindInterestingThing() != 0
            && WorldRandomRange(*GetPWorldSeed(), 100) < 20
            && PlaceNearInterestingThing();
    }
};

int main()
{
    CThing c, thing; c.Init(); thing.Init();
    CTCVillage village = { false };
    CTCVillager villager = { &village };
    CAIBrain brain = { &c };
    CAIStateGroup_LookAtInterestingThings g; g.PBrain = &brain; g.m_seed = 1; g.m_thing = 0;
    bool ok = !g.InitCreaturePosition() && g.m_seed == 1;     // no thing: no draw
    g.m_thing = &thing;
    int hits = 0;
    for (int i = 0; i < 400; ++i) {
        unsigned int s = g.m_seed; bool expect = WorldRandomRange(s, 100) < 20;
        bool r = g.InitCreaturePosition();
        if (r != expect) ok = false;
        if (r) ++hits;
    }
    ok = ok && hits > 0 && hits < 400 && g_placed == hits;
    c.Add(0x23, &villager);
    unsigned int before = g.m_seed;
    ok = ok && !g.InitCreaturePosition() && g.m_seed == before;   // outside curfew: left alone
    if (!ok) { std::printf("00919440_TEST FAIL\n"); return 1; }
    std::printf("00919440_TEST PASS\n");
    return 0;
}

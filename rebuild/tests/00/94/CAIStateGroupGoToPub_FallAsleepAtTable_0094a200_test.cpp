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

#include <cstring>
static unsigned int GFROR13(unsigned int v) { return (v >> 13) | (v << 19); }
inline unsigned int WorldRandomRange(unsigned int& seed, unsigned int range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range == 0)
        return 0;
    return seed % range;
}
struct CCharString { const char* text; CCharString(const char* t, long) : text(t) { } };

static const char* g_into; static const char* g_loop; static const char* g_out; static float g_time;
struct CAIStateGroup_GoToPub {
    unsigned int m_seed;
    unsigned int* GetPWorldSeed() { return &m_seed; }
    bool PlayAnims(const CCharString& a, const CCharString& b, const CCharString& c, float t)
    { g_into = a.text; g_loop = b.text; g_out = c.text; g_time = t; return t >= 0.0f; }
    bool FallAsleepAtTable()
    {
        float loopTime = (float)WorldRandomRange(*GetPWorldSeed(), 10);
        return PlayAnims(CCharString("SLUMPED_DRUNK_AT_TABLE_INTO", -1), CCharString("SLUMPED_DRUNK_AT_TABLE_LOOP", -1),
                         CCharString("SLUMPED_DRUNK_AT_TABLE_OUTOF", -1), loopTime);
    }
};

int main()
{
    CAIStateGroup_GoToPub g; g.m_seed = 12345;
    unsigned int expect = GFROR13(12345u * 0x24A1 + 0x24DF);
    bool r = g.FallAsleepAtTable();
    if (!r || g.m_seed != expect || g_time != (float)(expect % 10) || std::strcmp(g_into, "SLUMPED_DRUNK_AT_TABLE_INTO") != 0
        || std::strcmp(g_loop, "SLUMPED_DRUNK_AT_TABLE_LOOP") != 0 || std::strcmp(g_out, "SLUMPED_DRUNK_AT_TABLE_OUTOF") != 0) { std::printf("0094a200_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 50; ++i) { g.FallAsleepAtTable(); if (g_time < 0.0f || g_time >= 10.0f) { std::printf("0094a200_TEST FAIL\n"); return 1; } }
    std::printf("0094a200_TEST PASS\n");
    return 0;
}
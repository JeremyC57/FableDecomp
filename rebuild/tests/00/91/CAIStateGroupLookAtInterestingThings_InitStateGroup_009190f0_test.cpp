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
static long g_FramesPerSecond = 15, g_frame = 1000;
long GFGetCurrentFrame() { return g_frame; }
static void* g_registered = 0; static long g_type = 0;
struct CTCIdleScheduler { void RegisterIdleStateGroup(void* g, long t) { g_registered = g; g_type = t; } };

struct CAIStateGroup_LookAtInterestingThings {
    long FrameToLook;
    unsigned int m_seed;
    unsigned int* GetPWorldSeed() { return &m_seed; }
    void InitStateGroup(CThing& creature)
    {
        unsigned int a = WorldRandomRange(*GetPWorldSeed(), 240);
        unsigned int b = WorldRandomRange(*GetPWorldSeed(), 120);
        long framesPerSecond = g_FramesPerSecond;
        FrameToLook = GFGetCurrentFrame() + (a + b) * framesPerSecond;
        CTCIdleScheduler* scheduler;
        if (creature.GetInterface(&scheduler, 0xd3))
            scheduler->RegisterIdleStateGroup(this, 9);
    }
};

int main()
{
    CThing c; c.Init();
    CAIStateGroup_LookAtInterestingThings g; g.m_seed = 4242;
    unsigned int s = 4242; unsigned int a = WorldRandomRange(s, 240); unsigned int b = WorldRandomRange(s, 120);
    g.InitStateGroup(c);
    bool ok = g.FrameToLook == (long)(1000 + (a + b) * 15) && g.m_seed == s && g_registered == 0;
    CTCIdleScheduler sched; c.Add(0xd3, &sched);
    g.InitStateGroup(c);
    ok = ok && g_registered == &g && g_type == 9 && g.FrameToLook >= 1000 && g.FrameToLook < 1000 + 359 * 15;
    if (!ok) { std::printf("009190f0_TEST FAIL\n"); return 1; }
    std::printf("009190f0_TEST PASS\n");
    return 0;
}
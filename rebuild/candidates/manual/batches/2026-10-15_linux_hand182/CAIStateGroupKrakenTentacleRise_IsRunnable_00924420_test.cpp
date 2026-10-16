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

static long g_frame = 100;
long GFGetCurrentFrame() { return g_frame; }
struct CTCCreatureModeManager { long mode; bool HasMode(long m) { return m == mode; } };
struct CTCKrakenTentacle { long m_riseFrame; };

struct CAIStateGroup_KrakenTentacleRise {
    CAIBrain* PBrain;
    bool IsRunnable()
    {
        CTCCreatureModeManager* modes;
        if (!PBrain->PCreature->GetInterface(&modes, 0x31) || !modes->HasMode(0x54))
            return false;
        CTCKrakenTentacle* tentacle;
        if (!PBrain->PCreature->GetInterface(&tentacle, 0xe3))
            return false;
        return tentacle->m_riseFrame != 0 && GFGetCurrentFrame() > tentacle->m_riseFrame;
    }
};

int main()
{
    CThing t; t.Init();
    CTCCreatureModeManager modes = { 0 };
    CTCKrakenTentacle k = { 0 };
    CAIBrain brain = { &t };
    CAIStateGroup_KrakenTentacleRise g; g.PBrain = &brain;
    bool ok = !g.IsRunnable();
    t.Add(0x31, &modes); ok = ok && !g.IsRunnable();
    modes.mode = 0x54; ok = ok && !g.IsRunnable();
    t.Add(0xe3, &k); ok = ok && !g.IsRunnable();        // rise frame unset
    k.m_riseFrame = 100; ok = ok && !g.IsRunnable();    // not passed yet
    k.m_riseFrame = 99; ok = ok && g.IsRunnable();
    if (!ok) { std::printf("00924420_TEST FAIL\n"); return 1; }
    std::printf("00924420_TEST PASS\n");
    return 0;
}

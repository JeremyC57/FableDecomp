#include <cstdio>
// ---- test model: a thing's interface bitset + sorted {type, component} table, intelligent pointers ----
struct CThing;
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

struct CTCVillage { int dummy; };
struct CTCVillager { CTCVillage* village; };
CTCVillage* GFGetPVillageInterface(CTCVillager* v) { return v->village; }
struct CTCLight { bool m_lit; };

struct CAIStateGroup_LightHomeCandle {
    CAIBrain* PBrain;
    bool m_shouldBeLit;
    bool ShouldBeLit() { return m_shouldBeLit; }
    bool IsCandleInNeedOfChanging(CThing* candle)
    {
        CTCVillager* villager;
        PBrain->PCreature->GetInterface(&villager, 0x23);
        if (GFGetPVillageInterface(villager)) {
            CTCLight* light;
            candle->GetInterface(&light, 0x27);
            if (ShouldBeLit())
                return !light->m_lit;
            return light->m_lit;
        }
        return false;
    }
};

int main()
{
    CThing person, candle; person.Init(); candle.Init();
    CTCVillage village = { 0 };
    CTCVillager villager = { 0 };
    CTCLight light = { false };
    person.Add(0x23, &villager); candle.Add(0x27, &light);
    CAIBrain brain = { &person };
    CAIStateGroup_LightHomeCandle g; g.PBrain = &brain; g.m_shouldBeLit = true;
    bool ok = !g.IsCandleInNeedOfChanging(&candle);         // no village
    villager.village = &village;
    ok = ok && g.IsCandleInNeedOfChanging(&candle);         // should be lit, is not
    light.m_lit = true; ok = ok && !g.IsCandleInNeedOfChanging(&candle);
    g.m_shouldBeLit = false; ok = ok && g.IsCandleInNeedOfChanging(&candle);
    light.m_lit = false; ok = ok && !g.IsCandleInNeedOfChanging(&candle);
    if (!ok) { std::printf("00946730_TEST FAIL\n"); return 1; }
    std::printf("00946730_TEST PASS\n");
    return 0;
}
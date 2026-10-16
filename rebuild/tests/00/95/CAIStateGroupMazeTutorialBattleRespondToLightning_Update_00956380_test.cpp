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

static int g_look = 0, g_cast = 0, g_terminated = 0, g_stopAfterLook = 0;
static long g_spell = 0; static float g_duration = 0; static CThing* g_castTarget = 0;
struct CAction { long spell; float duration; CThing* target; CAction(void*, long s, float d, CThing* t) : spell(s), duration(d), target(t) { } };

struct CAIStateGroup_MazeTutorialBattleRespondToLightning {
    CBaseIntelligentPointer PTarget;
    bool IsStopping() { return g_stopAfterLook && g_look >= g_stopAfterLook; }
    void Terminate() { ++g_terminated; }
    int AIAction_LookAtThing(int, CBaseIntelligentPointer, long, float) { ++g_look; return 2; }
    int AIAction_PerformActionAndWait(const CAction& a, bool) { ++g_cast; g_spell = a.spell; g_duration = a.duration; g_castTarget = a.target; return 2; }
    void Update()
    {
        if (IS_THING_ALIVE(PTarget) && !IsStopping()) {
            AIAction_LookAtThing(1, PTarget, 0x21, 0.083333336f);
            if (IS_THING_ALIVE(PTarget) && !IsStopping())
                AIAction_PerformActionAndWait(CAction(0, 2, 3.0f, PTarget.GetPItem()), false);
        }
        Terminate();
    }
};

int main()
{
    CThing target; target.Init();
    CAIStateGroup_MazeTutorialBattleRespondToLightning g; g.PTarget.SetPItem(0);
    g.Update();
    bool ok = g_look == 0 && g_cast == 0 && g_terminated == 1;
    g.PTarget.SetPItem(&target);
    g.Update();
    ok = ok && g_look == 1 && g_cast == 1 && g_spell == 2 && g_duration == 3.0f && g_castTarget == &target && g_terminated == 2;
    g_stopAfterLook = 2;
    g.Update();
    ok = ok && g_look == 2 && g_cast == 1 && g_terminated == 3;
    if (!ok) { std::printf("00956380_TEST FAIL\n"); return 1; }
    std::printf("00956380_TEST PASS\n");
    return 0;
}
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

static int g_look = 0, g_move = 0, g_terminated = 0, g_lookRet = 2, g_moveRet = 2;
static float g_prox = 0; static long g_frames = 0;
struct CThingGuard { CThing* target; CThing* GetPGuardTarget() { return target; } };

struct CAIStateGroup_Guard {
    CThingGuard* creature;
    int AIAction_LookAtThing(int, CBaseIntelligentPointer t, long frames, float) { ++g_look; g_frames = frames; return t.GetPItem() ? g_lookRet : 0; }
    int AIAction_MoveToThingProximity(int, CBaseIntelligentPointer t, float prox, bool* arrived, int, bool, bool, bool, bool, float)
    { ++g_move; g_prox = prox; if (*arrived) return 0; return t.GetPItem() ? g_moveRet : 0; }
    void Terminate() { ++g_terminated; }
    void Update()
    {
        CBaseIntelligentPointer target; target.SetPItem(creature->GetPGuardTarget());
        if (target.GetPItem()) {
            if (AIAction_LookAtThing(1, target, 0x11, 0.0027777f) == 2) {
                bool arrived = false;
                if (AIAction_MoveToThingProximity(1, target, 2.0f, &arrived, 3, true, false, true, true, 0.0f) == 2)
                    Terminate();
            }
        }
    }
};

int main()
{
    CThing t; t.Init();
    CThingGuard guard = { 0 };
    CAIStateGroup_Guard g; g.creature = &guard;
    g.Update();
    if (g_look != 0 || g_terminated != 0) { std::printf("00911630_TEST FAIL\n"); return 1; }
    guard.target = &t; g_lookRet = 1;
    g.Update();
    if (g_look != 1 || g_move != 0 || g_frames != 0x11) { std::printf("00911630_TEST FAIL\n"); return 1; }
    g_lookRet = 2;
    g.Update();
    if (g_move != 1 || g_terminated != 1 || g_prox != 2.0f) { std::printf("00911630_TEST FAIL\n"); return 1; }
    std::printf("00911630_TEST PASS\n");
    return 0;
}

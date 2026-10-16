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

static int g_released = 0, g_baseCalls = 0;
struct CTCGameTable { int id; void ReleaseGameMaster() { g_released += id; } };
struct CTCGameState { void* m_pGameMaster; };

struct CAIStateGroup_GameMaster {
    CAIBrain* PBrain;
    CBaseIntelligentPointer PTable;
    void BaseOnReplace() { ++g_baseCalls; }
    void OnReplace()
    {
        if (PTable.GetPItem()) {
            CTCGameTable* table;
            PTable.GetPItem()->GetInterface(&table, 0xcf);
            table->ReleaseGameMaster();
        }
        BaseOnReplace();
        CTCGameState* state;
        PBrain->PCreature->GetInterface(&state, 0xd2);
        state->m_pGameMaster = 0;
    }
};

int main()
{
    CThing creature, tableThing;
    creature.Init(); tableThing.Init();
    CTCGameTable table = { 5 };
    CTCGameState state = { &table };
    tableThing.Add(0x10, 0); tableThing.Add(0xcf, &table);
    creature.Add(0xd2, &state); creature.Add(0x23, 0);
    CAIBrain brain = { &creature };
    CAIStateGroup_GameMaster g; g.PBrain = &brain; g.PTable.SetPItem(&tableThing);
    g.OnReplace();
    if (g_released != 5 || g_baseCalls != 1 || state.m_pGameMaster != 0) { std::printf("0094ca80_TEST FAIL\n"); return 1; }
    state.m_pGameMaster = &table; g.PTable.SetPItem(0);
    g.OnReplace();
    if (g_released != 5 || g_baseCalls != 2 || state.m_pGameMaster != 0) { std::printf("0094ca80_TEST FAIL\n"); return 1; }
    std::printf("0094ca80_TEST PASS\n");
    return 0;
}
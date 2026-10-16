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

struct C3DVector { float x, y, z; };
struct CTCBoss { long phase; bool force; void SetNextBossPhase(long p, bool f) { phase = p; force = f; } };
struct CThingAICreature { C3DVector pos; const C3DVector& GetPosition() { return pos; } };
static int g_baseInit = 0;

struct CAIStateGroup_HoverHornetQueen {
    CAIBrain* PBrain;
    C3DVector StartPos;
    bool bOneTime;
    long m_phaseCounter;
    long EnemyKnockDownFrame;
    void BaseInitialise(CAIBrain& brain, CThingAICreature&, long, long, const char*) { PBrain = &brain; ++g_baseInit; }
    void Initialise(CAIBrain& brain, CThingAICreature& creature, long id, long priority, const char* name)
    {
        StartPos = creature.GetPosition();
        BaseInitialise(brain, creature, id, priority, name);
        bOneTime = true;
        EnemyKnockDownFrame = 0;
        m_phaseCounter = 0;
        CTCBoss* boss;
        PBrain->PCreature->GetInterface(&boss, 0xad);
        boss->SetNextBossPhase(0, false);
    }
};

int main()
{
    CThing thing; thing.Init();
    CTCBoss boss = { 3, true };
    thing.Add(0xad, &boss);
    CAIBrain brain = { &thing };
    CThingAICreature creature = { { 1.0f, 2.0f, 3.0f } };
    CAIStateGroup_HoverHornetQueen q; q.bOneTime = false; q.m_phaseCounter = 7; q.EnemyKnockDownFrame = 9;
    q.Initialise(brain, creature, 1, 2, "q");
    if (q.StartPos.y != 2.0f || !q.bOneTime || q.m_phaseCounter != 0 || q.EnemyKnockDownFrame != 0
        || boss.phase != 0 || boss.force || g_baseInit != 1) { std::printf("0095a890_TEST FAIL\n"); return 1; }
    std::printf("0095a890_TEST PASS\n");
    return 0;
}

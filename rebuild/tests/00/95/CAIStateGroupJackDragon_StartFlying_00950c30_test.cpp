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

static int g_destroyed = 0, g_idles = 0, g_modes = 0;
struct SCountBlock { long m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };
static void DestroyAction(void*) { ++g_destroyed; }
struct CCountedActionPointer {
    void* m_pObject; SCountBlock* m_pCount;
    CCountedActionPointer(void* o, SCountBlock* c) : m_pObject(o), m_pCount(c) { }
    CCountedActionPointer(const CCountedActionPointer& o) : m_pObject(o.m_pObject), m_pCount(o.m_pCount) { if (m_pCount) ++m_pCount->m_references; }
    ~CCountedActionPointer()
    {
        if (m_pCount) {
            if (--m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; }
        }
    }
};
struct CModeDragonFlying {
    float speed; CCountedActionPointer action;
    CModeDragonFlying(void*, float s, CCountedActionPointer a) : speed(s), action(a) { }
};
static CModeDragonFlying* g_last = 0;
struct CTCCreatureModeManager { bool AddMode(CModeDragonFlying* m) { ++g_modes; delete g_last; g_last = m; return true; } };

struct CAIStateGroup_JackDragon {
    CAIBrain* PBrain;
    long AIAction_DoNothing() { ++g_idles; return 0; }
    void StartFlying(void* target, CCountedActionPointer action)
    {
        CTCCreatureModeManager* modes;
        if (PBrain->PCreature->GetInterface(&modes, 0x31)) {
            modes->AddMode(new CModeDragonFlying(target, 10.0f, action));
            AIAction_DoNothing();
        }
    }
};

int main()
{
    CThing dragon; dragon.Init();
    CAIBrain brain = { &dragon };
    CAIStateGroup_JackDragon d; d.PBrain = &brain;
    SCountBlock* block = new SCountBlock; block->m_references = 1; block->m_pfnDestroy = DestroyAction; block->m_pObject = 0;
    {
        CCountedActionPointer act(0, block);
        d.StartFlying(0, act);                  // no mode manager: nothing happens
        if (g_modes != 0 || g_idles != 0 || block->m_references != 1) { std::printf("00950c30_TEST FAIL\n"); return 1; }
        CTCCreatureModeManager modes;
        dragon.Add(0x31, &modes);
        d.StartFlying(0, act);
        if (g_modes != 1 || g_idles != 1 || g_last->speed != 10.0f || block->m_references != 2) { std::printf("00950c30_TEST FAIL\n"); return 1; }
    }
    delete g_last; g_last = 0;
    if (g_destroyed != 1) { std::printf("00950c30_TEST FAIL\n"); return 1; }
    std::printf("00950c30_TEST PASS\n");
    return 0;
}
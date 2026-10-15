#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
#define FASTCALL __fastcall
#else
#define FASTCALL
#endif
struct CTCBase { long type; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
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
    CInterfaceMap::SEntry m_entries[4];
    long m_entryCount;
    CThing() : m_entryCount(0) { for (int i = 0; i < 9; ++i) m_presentBits[i] = 0; m_interfaces.first = m_entries; m_interfaces.last = m_entries; }
    void Add(long type, CTCBase* component)
    {
        m_presentBits[type >> 5] |= (1UL << (type & 31));
        int i = m_entryCount;
        while (i > 0 && m_entries[i - 1].type > type) { m_entries[i] = m_entries[i - 1]; --i; }
        m_entries[i].type = type; m_entries[i].component = component;
        ++m_entryCount; m_interfaces.last = m_entries + m_entryCount;
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
    virtual ~CBaseIntelligentPointer() { }
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    CThing* GetPItem() const { return m_pItem; }
};
struct CMode { virtual ~CMode() { } virtual long GetModeType() = 0; };
struct CTestMode : CMode { long m_type; int m_queries; CTestMode(long t) : m_type(t), m_queries(0) { } virtual long GetModeType() { ++m_queries; return m_type; } };
struct CTCCreatureModeManager : CTCBase {
    CMode* m_current; int m_restarts; unsigned long m_added[4]; int m_addCount; unsigned long m_removed[4]; int m_removeCount;
    CTCCreatureModeManager(CMode* current) : m_current(current), m_restarts(0), m_addCount(0), m_removeCount(0) { }
    CMode* GetCurrentMode() { return m_current; }
    void RestartCurrentMode() { ++m_restarts; }
    bool AddMode(unsigned long mode) { if (m_addCount < 4) m_added[m_addCount] = mode; ++m_addCount; return true; }
    void RemoveMode(unsigned long mode) { if (m_removeCount < 4) m_removed[m_removeCount] = mode; ++m_removeCount; }
};
static int g_baseInit = 0;
struct CCreatureActionBase { virtual ~CCreatureActionBase() { } virtual void Initialise() { ++g_baseInit; } CBaseIntelligentPointer PCreature; };
struct CCreatureAction_BeStunned : CCreatureActionBase { virtual void Initialise(); };
void CCreatureAction_BeStunned::Initialise()
{
    CTCCreatureModeManager* pModes;
    PCreature.GetPItem()->GetInterface(&pModes, 49);
    pModes->GetCurrentMode()->GetModeType();
    pModes->RemoveMode(31);
    pModes->RemoveMode(29);
    pModes->AddMode(29);
    CCreatureActionBase::Initialise();
}
int main()
{
    CTestMode current(5);
    CThing creature; CTCCreatureModeManager modes(&current); creature.Add(49, &modes); creature.Add(60, 0);
    CCreatureAction_BeStunned action; action.PCreature.m_pItem = &creature;
    action.Initialise();
    bool ok = g_baseInit == 1 && current.m_queries == 1 && modes.m_removeCount == 2 && modes.m_removed[0] == 31 && modes.m_removed[1] == 29
        && modes.m_addCount == 1 && modes.m_added[0] == 29 && modes.m_restarts == 0;
    if (!ok) { std::printf("008eda10_TEST FAIL\n"); return 1; }
    std::printf("008eda10_TEST PASS\n");
    return 0;
}

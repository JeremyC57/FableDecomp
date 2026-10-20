// Behaviour test for 00848fd0: body copied from src/00848fd0.cpp (CCreatureAction_SitDown::PerformAction), the callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
};

static int g_getPItem = 0;
struct CBaseIntelligentPointer { virtual void v00() {} CThing* m_pItem; CThing* GetPItem() const { ++g_getPItem; return m_pItem; } };
struct CModeBase;
struct SCountBlock { long m_references; void (__fastcall* m_pfnDestroy)(CModeBase*); CModeBase* m_pObject; };
static int g_cpDtor = 0;
struct CCountedPointer_CModeBase {
    CModeBase*   m_pObject;
    SCountBlock* m_pCount;
    CCountedPointer_CModeBase(CModeBase* o, SCountBlock* c) : m_pObject(o), m_pCount(c) { }
    CCountedPointer_CModeBase(const CCountedPointer_CModeBase& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount) { if (m_pCount) ++m_pCount->m_references; }
    ~CCountedPointer_CModeBase() { ++g_cpDtor; if (m_pCount) --m_pCount->m_references; }
};
static int g_addMode = 0; static CModeBase* g_addedMode = 0; static void* g_addThis = 0;
struct CTCCreatureModeManager { bool AddMode(CCountedPointer_CModeBase mode) { ++g_addMode; g_addedMode = mode.m_pObject; g_addThis = this; CHECK(mode.m_pCount->m_references == 2); return true; } };
struct CCreatureActionBase {
    virtual void v00() {} virtual void v01() {} virtual void v02() {}
    virtual void PerformAction() {}
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;
    unsigned char _pad10[0xa8 - 0x10];
};
struct CCreatureAction_SitDown : CCreatureActionBase {
    CCountedPointer_CModeBase PMode;
    CCreatureAction_SitDown(CModeBase* o, SCountBlock* c) : PMode(o, c) { }
    virtual void PerformAction();
};
void CCreatureAction_SitDown::PerformAction()
{
    CTCCreatureModeManager* pModes;
    CThing* creature = PCreature.GetPItem();
    if (creature->HasTC(0x31))
        pModes = creature->GetTC<CTCCreatureModeManager>(0x31);
    pModes->AddMode(PMode);
}
int main()
{
    CThing creature; memset(&creature, 0, sizeof(creature)); CTCCreatureModeManager modes;
    CInterfaceMap::SEntry entries[2] = { { 0x10, (CTCBase*)0x99 }, { 0x31, (CTCBase*)&modes } };
    creature.m_interfaces.first = entries; creature.m_interfaces.last = entries + 2;
    creature.m_presentBits[0x31 >> 5] = 1UL << (0x31 & 31);   // a creature always carries its mode manager (the miss arm uses an unset pointer)
    SCountBlock block; block.m_references = 1; block.m_pfnDestroy = 0; block.m_pObject = (CModeBase*)0x1234;
    CCreatureAction_SitDown action((CModeBase*)0x1234, &block); action.PCreature.m_pItem = &creature;
    action.PerformAction();
    CHECK(g_getPItem == 1 && g_addMode == 1 && g_addThis == &modes && g_addedMode == (CModeBase*)0x1234);
    CHECK(g_cpDtor == 1 && block.m_references == 1);          // the by-value copy was made and destroyed
    if (fails == 0) printf("00848fd0_TEST PASS\n"); else printf("00848fd0_TEST FAIL\n");
    return fails;
}
#include <cstdio>
#include <cstring>

struct CThing;
struct CBaseObject { int id; };
static int g_registered = 0, g_unregistered = 0, g_tcBaseDtors = 0;
struct CTCBase {
    virtual ~CTCBase() { ++g_tcBaseDtors; }
    CThing* ParentThing;
    bool ToBeRemoved, IsInGlobalUpdateTCs;
    CTCBase(CThing& owner) : ParentThing(&owner), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { }
    void AddToGlobalTCs() { IsInGlobalUpdateTCs = true; ++g_registered; }
    void RemoveFromGlobalTCs() { IsInGlobalUpdateTCs = false; ++g_unregistered; }
};
static int g_defDestroys = 0;
struct CDefPointee { long m_refCount; CDefPointee() : m_refCount(1) { } void Destroy() { ++g_defDestroys; } };
struct CDefPointer {
    CDefPointee* m_pDef;
    CDefPointer() : m_pDef(0) { }
    ~CDefPointer() { CDefPointee* def = m_pDef; if (def) { if (--def->m_refCount == 0) def->Destroy(); m_pDef = 0; } }
};
static int g_ipDtors = 0;
struct CIntelligentPointer {
    CBaseObject* m_pItem;
    CIntelligentPointer() : m_pItem(0) { }
    CIntelligentPointer(CBaseObject* item) : m_pItem(0) { SetPItem(item); }
    ~CIntelligentPointer() { ++g_ipDtors; }
    void SetPItem(CBaseObject* item) { m_pItem = item; }
    CBaseObject* GetPItem() const { return m_pItem; }
};
struct CThing { int id; };

struct CTCWillEnflame : CTCBase {
    bool EnflameActionInitialised, Released; CDefPointer PDef; long SpellLevel; CIntelligentPointer PItemRemoved; bool IsZTargetting; CIntelligentPointer PTarget;
    CTCWillEnflame(CThing& owner) : CTCBase(owner) { }
};
int main()
{
    CThing thing; thing.id = 3;
    CDefPointee def; def.m_refCount = 2;
    CTCWillEnflame* s = new CTCWillEnflame(thing);
    s->PDef.m_pDef = &def;
    delete s;
    if (g_ipDtors != 2 || def.m_refCount != 1 || g_defDestroys != 0 || g_tcBaseDtors != 1) { std::printf("007a50f0_TEST FAIL\n"); return 1; }
    CTCWillEnflame* t = new CTCWillEnflame(thing);
    t->PDef.m_pDef = &def;
    delete t;
    if (g_ipDtors != 4 || def.m_refCount != 0 || g_defDestroys != 1 || g_tcBaseDtors != 2) { std::printf("007a50f0_TEST FAIL\n"); return 1; }
    std::printf("007a50f0_TEST PASS\n");
    return 0;
}
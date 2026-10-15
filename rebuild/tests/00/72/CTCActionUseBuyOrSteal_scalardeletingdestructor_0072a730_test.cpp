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

static char g_log[64]; static int g_logLen = 0;
static void Log(char c) { g_log[g_logLen++] = c; g_log[g_logLen] = 0; }
struct CTCActionUseBase : CTCBase {
    CDefPointer PActionUseDef; bool Usable;
    CTCActionUseBase(CThing& owner) : CTCBase(owner), Usable(true) { }
    ~CTCActionUseBase() { Log('D'); }        /* the def release runs after this body */
};
struct CTCActionUseBuyOrSteal : CTCActionUseBase {
    CIntelligentPointer PPerformer; bool WaitingForConfirmation;
    CTCActionUseBuyOrSteal(CThing& owner) : CTCActionUseBase(owner), WaitingForConfirmation(false) { AddToGlobalTCs(); }
    ~CTCActionUseBuyOrSteal() { Log('R'); RemoveFromGlobalTCs(); }
};
int main()
{
    CThing thing; thing.id = 1;
    CDefPointee def; def.m_refCount = 1;
    CDefPointee shared; shared.m_refCount = 2;
    CTCActionUseBuyOrSteal* a = new CTCActionUseBuyOrSteal(thing);
    a->PActionUseDef.m_pDef = &def;
    delete a;
    if (g_unregistered != 1 || g_ipDtors != 1 || g_defDestroys != 1 || def.m_refCount != 0 || g_tcBaseDtors != 1) { std::printf("0072a730_TEST FAIL\n"); return 1; }
    CTCActionUseBuyOrSteal* b = new CTCActionUseBuyOrSteal(thing);
    b->PActionUseDef.m_pDef = &shared;
    delete b;
    if (g_defDestroys != 1 || shared.m_refCount != 1 || g_unregistered != 2 || g_tcBaseDtors != 2) { std::printf("0072a730_TEST FAIL\n"); return 1; }
    if (std::strcmp(g_log, "RDRD") != 0) { std::printf("0072a730_TEST FAIL\n"); return 1; }
    std::printf("0072a730_TEST PASS\n");
    return 0;
}
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

struct CTCActionUseBase : CTCBase {
    CDefPointer PActionUseDef; bool Usable;
    CTCActionUseBase(CThing& owner) : CTCBase(owner), Usable(true) { }
};
struct CTCActionUseBuyOrSteal : CTCActionUseBase {
    CIntelligentPointer PPerformer; bool WaitingForConfirmation;
    CTCActionUseBuyOrSteal(CThing& owner) : CTCActionUseBase(owner), PPerformer(0), WaitingForConfirmation(false) { AddToGlobalTCs(); }
};
int main()
{
    CThing thing; thing.id = 7;
    CTCActionUseBuyOrSteal c(thing);
    if (c.ParentThing != &thing || c.PActionUseDef.m_pDef != 0 || !c.Usable || c.PPerformer.GetPItem() != 0) { std::printf("0072a630_TEST FAIL\n"); return 1; }
    if (c.WaitingForConfirmation || !c.IsInGlobalUpdateTCs || g_registered != 1) { std::printf("0072a630_TEST FAIL\n"); return 1; }
    std::printf("0072a630_TEST PASS\n");
    return 0;
}
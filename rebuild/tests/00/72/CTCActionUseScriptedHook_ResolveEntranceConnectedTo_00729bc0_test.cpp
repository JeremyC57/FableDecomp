// Behaviour test for 00729bc0: the model below is a copy of src/00729bc0.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
static int g_ctors = 0, g_dtors = 0, g_sets = 0, g_gets = 0, g_lookups = 0;
struct CThing {};
struct CBaseObject {};
struct CBaseIntelligentPointer {
    CBaseObject* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_ctors; }
    virtual ~CBaseIntelligentPointer() { ++g_dtors; }
    CBaseObject* GetPItem() const { ++g_gets; return m_pItem; }
    void SetPItem(CBaseObject* item) { ++g_sets; m_pItem = item; }
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() : CBaseIntelligentPointer() {}
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { CBaseObject* item = other.GetPItem(); SetPItem(item); return *this; }
};
static CThing g_entrance; static unsigned __int64 g_lookedUp = 0; static bool g_found = true;
struct CThingManager {
    CThing* GetThingByUID(unsigned __int64 uid) { ++g_lookups; g_lookedUp = uid; return g_found ? &g_entrance : 0; }
};
static CThingManager g_things;
struct CTCActionUseScriptedHook {
    void*   __vftable;
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    unsigned char _pad0a[2];
    bool    Usable;
    bool    ForceConfirmation;
    bool    WaitingForConfirmation;
    bool    PerformAction;
    bool    TeleportToRegionEntrance;
    unsigned __int64 EntranceConnectedToUID;
    CIntelligentPointer<CThing> PEntranceConnectedTo;
    CThingManager* GetThingManager() { return &g_things; }
    void ResolveEntranceConnectedTo();
};

void CTCActionUseScriptedHook::ResolveEntranceConnectedTo()
{
    PEntranceConnectedTo = CIntelligentPointer<CThing>(GetThingManager()->GetThingByUID(EntranceConnectedToUID));
    if (PEntranceConnectedTo.GetPItem())
        PEntranceConnectedTo.GetPItem();
}

static bool Run(bool found, unsigned __int64 uid, int expectGets)
{
    CTCActionUseScriptedHook h; h.__vftable = 0; h.ParentThing = 0; h.EntranceConnectedToUID = uid;
    g_found = found; g_ctors = g_dtors = g_sets = g_gets = g_lookups = 0; g_lookedUp = 0;
    h.ResolveEntranceConnectedTo();
    if (g_lookups != 1 || g_lookedUp != uid) return false;
    if (h.PEntranceConnectedTo.m_pItem != (found ? (CBaseObject*)&g_entrance : 0)) return false;
    if (g_ctors != 1 || g_dtors != 1 || g_sets != 2) return false;   // one temporary, set once, copied into the member
    if (g_gets != expectGets) return false;
    return true;
}
int main()
{
    bool ok = Run(true, 0x1122334455667788ULL, 3) && Run(false, 42, 2);   // found: the item is read twice after the copy; missing: once
    if (!ok) { std::printf("00729bc0_TEST FAIL\n"); return 1; }
    std::printf("00729bc0_TEST PASS\n");
    return 0;
}
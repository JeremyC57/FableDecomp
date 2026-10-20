// Behaviour test for CTCCarriedActionUseSpade::vfunc_28 (0x007F03D0): a user able to dig is given an activate-quest action
// named "Expression_Dig" (no object, no resource load); the temporaries die in reverse order; the spade's 0x1a component
// is notified whenever present.
#include <cstdio>
#include <cstring>
struct CThing;
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* p = first;
        while (p != last && p->type < *type) ++p;
        return p;
    }
};
static char g_log[64];
static void Log(char c) { size_t n = std::strlen(g_log); g_log[n] = c; g_log[n + 1] = 0; }
struct CCharString {
    const char* m_pData;
    CCharString(const char* text, long length) : m_pData(text) { Log('s'); }
    CCharString(const CCharString& o) : m_pData(o.m_pData) { Log('c'); }
    ~CCharString() { Log('~'); }
};
struct CBaseIntelligentPointer {
    virtual void v00() {}
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    ~CBaseIntelligentPointer() { Log('p'); }
};
struct CIntelligentPointer : CBaseIntelligentPointer { };
struct CCreatureActionBase {
    virtual void v00() {}
    ~CCreatureActionBase() { Log('b'); }
};
struct CThingCreatureBase;
struct CCreatureAction_ActivateQuest : CCreatureActionBase {
    CCharString         ScriptName;
    bool                LoadResources;
    CIntelligentPointer PObjectToUse;
    CThingCreatureBase* Owner;
    CCreatureAction_ActivateQuest(CThingCreatureBase* owner, CThing* objectToUse, const CCharString& scriptName, bool loadResources)
        : ScriptName(scriptName), LoadResources(loadResources), Owner(owner) { PObjectToUse.m_pItem = objectToUse; Log('a'); }
};
template <class T> T* AddressOf(const T& t) { return const_cast<T*>(&t); }
struct CThingCreatureBase {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    bool CanDig;
    const char* ActionName; bool ActionLoad; void* ActionObject; CThingCreatureBase* ActionOwner;
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    bool SetCurrentAction(CCreatureActionBase* action)
    {
        CCreatureAction_ActivateQuest* a = (CCreatureAction_ActivateQuest*)action;
        ActionName = a->ScriptName.m_pData; ActionLoad = a->LoadResources; ActionObject = a->PObjectToUse.m_pItem; ActionOwner = a->Owner;
        Log('S');
        return true;
    }
};
bool IsThingAbleToDig(CThingCreatureBase* thing) { return thing->CanDig; }
struct CTCShowableItem {
    int Calls;
    void OnShowEnded() { ++Calls; }
};
struct CTCCarriedActionUseBase {
    virtual void v00() {}
    CThingCreatureBase* ParentThing;
    int BaseCalls;
    void OnUseBase(CThingCreatureBase* user) { ++BaseCalls; }
};
struct CTCCarriedActionUseSpade : CTCCarriedActionUseBase {
    void vfunc_28(CThingCreatureBase* user);
};

void CTCCarriedActionUseSpade::vfunc_28(CThingCreatureBase* user)
{
    CTCCarriedActionUseBase::OnUseBase(user);
    if (IsThingAbleToDig(user)) {
        CCharString name("Expression_Dig", -1);
        user->SetCurrentAction(AddressOf(CCreatureAction_ActivateQuest(user, 0, name, false)));
    }
    if (ParentThing->HasTC(0x1a))
        ((CTCShowableItem*)ParentThing->GetTC(0x1a))->OnShowEnded();
}

int main()
{
    int bad = 0;
    CThingCreatureBase user; std::memset(&user, 0, sizeof user);
    CThingCreatureBase spade; std::memset(&spade, 0, sizeof spade);
    CTCShowableItem notify; notify.Calls = 0;
    CInterfaceMap::SEntry e[2];
    e[0].type = 0x05; e[0].component = 0;
    e[1].type = 0x1a; e[1].component = (CTCBase*)&notify;
    spade.m_interfaces.first = e; spade.m_interfaces.last = e + 2;
    spade.m_presentBits[0] = 0x4000000;
    CTCCarriedActionUseSpade action; action.ParentThing = &spade; action.BaseCalls = 0;
    user.CanDig = true; g_log[0] = 0;
    action.vfunc_28(&user);
    if (std::strcmp(user.ActionName, "Expression_Dig") || user.ActionLoad || user.ActionObject || user.ActionOwner != &user) bad |= 1;
    if (std::strcmp(g_log, "scaSp~b~") || notify.Calls != 1 || action.BaseCalls != 1) bad |= 2;
    user.CanDig = false; user.ActionName = 0; g_log[0] = 0; spade.m_presentBits[0] = 0;
    action.vfunc_28(&user);
    if (user.ActionName || g_log[0] || notify.Calls != 1 || action.BaseCalls != 2) bad |= 4;
    if (bad) { std::printf("007f03d0_TEST FAIL\n"); return 1; }
    std::printf("007f03d0_TEST PASS\n");
    return 0;
}

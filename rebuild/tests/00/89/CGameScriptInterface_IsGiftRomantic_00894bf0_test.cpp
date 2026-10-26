// Behaviour test for 00894bf0: body copied from src/00894bf0.cpp (CGameScriptInterface::IsGiftRomantic), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CCharString {
public:
    void* PStringData;
};
static int g_destroyed = 0;
struct CDefObject_Retail {
    virtual void v000() {}
    virtual void Destroy() { ++g_destroyed; }
    long m_refCount;
};
struct CGiftDef_Retail : CDefObject_Retail {
    unsigned char _pad08[0x20];
    long          m_giftType;
};
template<class T> struct CDefPointerSlot {
    T* Object;
    CDefPointerSlot() : Object(0) { }
    ~CDefPointerSlot()
    {
        if (Object) {
            if (--Object->m_refCount == 0)
                Object->Destroy();
        }
    }
    T* operator->() const { return Object; }
};
static CGiftDef_Retail* g_gift = 0; static int g_getSubDef = 0;
struct CDefClass_Retail : CDefObject_Retail {
    bool GetSubDef(CDefPointerSlot<CGiftDef_Retail>* out)
    {
        ++g_getSubDef;
        if (!g_gift) return false;
        ++g_gift->m_refCount; out->Object = g_gift; return true;
    }
};
static CDefClass_Retail* g_class = 0; static int g_getDef = 0; static long g_getDefIndex = -1;
struct CGameDefinitionManager_Retail {
    static CGameDefinitionManager_Retail& Get();
    bool GetDef(long index, CDefPointerSlot<CDefClass_Retail>* out)
    {
        ++g_getDef; g_getDefIndex = index;
        if (!g_class) return false;
        ++g_class->m_refCount; out->Object = g_class; return true;
    }
};
static CGameDefinitionManager_Retail g_gameDefs;
CGameDefinitionManager_Retail& CGameDefinitionManager_Retail::Get() { return g_gameDefs; }
static int g_nameLookups = 0; static const void* g_lookedUp = 0;
struct CDefinitionManager_Retail {
    long GetDefGlobalIndexFromName(const CCharString& name) { ++g_nameLookups; g_lookedUp = name.PStringData; return 606; }
};
static CDefinitionManager_Retail g_defs;
class CWorld;
class CGameScriptInterface { public:
    virtual void v000() {}
    CWorld*                    World;
    void*                      Component;
    void*                      DisplayEngine;
    CDefinitionManager_Retail* DefinitionManager;
    void*                      PlayerManager;
    bool IsGiftRomantic(const CCharString& defName) const;
};
bool CGameScriptInterface::IsGiftRomantic(const CCharString& defName) const
{
    long index = DefinitionManager->GetDefGlobalIndexFromName(defName);
    CDefPointerSlot<CDefClass_Retail> defClass;
    if (CGameDefinitionManager_Retail::Get().GetDef(index, &defClass)) {
        CDefPointerSlot<CGiftDef_Retail> giftDef;
        if (defClass->GetSubDef(&giftDef))
            return giftDef->m_giftType == 1;
    }
    return false;
}
int main()
{
    static CDefClass_Retail classDef; static CGiftDef_Retail gift; classDef.m_refCount = 1; gift.m_refCount = 1;
    CGameScriptInterface gsi; memset(&gsi, 0, sizeof(gsi)); gsi.DefinitionManager = &g_defs;
    CCharString name = { (void*)"OBJECT_GIFT" };
    // unknown def: false, no sub-def lookup
    g_class = 0; CHECK(!gsi.IsGiftRomantic(name)); CHECK(g_nameLookups == 1 && g_lookedUp == name.PStringData && g_getDef == 1 && g_getDefIndex == 606 && g_getSubDef == 0);
    // def without a gift sub-def: false, the class def released
    g_class = &classDef; g_gift = 0; CHECK(!gsi.IsGiftRomantic(name)); CHECK(g_getSubDef == 1 && classDef.m_refCount == 1 && g_destroyed == 0);
    // gift of the matching type: true; both defs released
    g_gift = &gift; gift.m_giftType = 1; CHECK(gsi.IsGiftRomantic(name)); CHECK(classDef.m_refCount == 1 && gift.m_refCount == 1 && g_destroyed == 0);
    // gift of another type: false
    gift.m_giftType = 1 + 1; CHECK(!gsi.IsGiftRomantic(name)); CHECK(gift.m_refCount == 1);
    // defs held only by the lookups are destroyed
    classDef.m_refCount = 0; gift.m_refCount = 0; gift.m_giftType = 1; CHECK(gsi.IsGiftRomantic(name)); CHECK(g_destroyed == 2);
    if (fails == 0) printf("00894bf0_TEST PASS\n"); else printf("00894bf0_TEST FAIL\n");
    return fails;
}
// Behaviour test for 00772610: body copied from src/00772610.cpp (CTCShop::GetName), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_deleted = 0;
struct CDefPointeeBase {
    virtual void Destroy() {}
    virtual void Delete() { ++g_deleted; }
    long m_refCount;
    void ReleaseReference() { if (--m_refCount == 0) Delete(); }
};
struct CShopDef : CDefPointeeBase {
    unsigned char _pad08[0x28 - 0x08];
    long Name;
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->ReleaseReference(); }
    T* operator->() const { return Object; }
};
static CShopDef* g_shopDef = 0; static int g_getSubDef = 0;
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CShopDef>* out)
    {
        ++g_getSubDef;
        if (!g_shopDef) return false;
        ++g_shopDef->m_refCount; out->Object = g_shopDef; return true;
    }
};
static CDefClassBase* g_thingDef = 0; static int g_getDef = 0; static long g_getDefIndex = -1;
struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<CDefClassBase>* out)
    {
        ++g_getDef; g_getDefIndex = index;
        ++g_thingDef->m_refCount; out->Object = g_thingDef; return true;
    }
};
static CDefinitionManager g_manager;
struct CThing {
    unsigned char  _pad00[0x8c];
    unsigned short DefIndex;
};
struct CTCShop {
    virtual void v00() {}
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    CDefinitionManager* GetDefinitions() const { return &g_manager; }
    long GetName() const;
};
long CTCShop::GetName() const
{
    CDefPointer<CDefClassBase> thingDef;
    GetDefinitions()->GetDef(ParentThing->DefIndex, &thingDef);
    CDefPointer<CShopDef> shopDef;
    if (thingDef->GetSubDef(&shopDef))
        return shopDef->Name;
    return 0;
}
int main()
{
    static CThing thing; static CDefClassBase thingDef; static CShopDef shop; thingDef.m_refCount = 1; shop.m_refCount = 1; shop.Name = 0x4e414d45; g_thingDef = &thingDef; thing.DefIndex = 0x1f3;
    CTCShop s; s.ParentThing = &thing;
    // no shop sub-def: 0, the thing def released
    g_shopDef = 0; CHECK(s.GetName() == 0); CHECK(g_getDef == 1 && g_getDefIndex == 0x1f3 && g_getSubDef == 1 && thingDef.m_refCount == 1 && g_deleted == 0);
    // shop sub-def: its name word, both defs released
    g_shopDef = &shop; CHECK(s.GetName() == 0x4e414d45); CHECK(thingDef.m_refCount == 1 && shop.m_refCount == 1 && g_deleted == 0);
    // defs held only by the lookups are deleted
    thingDef.m_refCount = 0; shop.m_refCount = 0; CHECK(s.GetName() == 0x4e414d45); CHECK(g_deleted == 2);
    if (fails == 0) printf("00772610_TEST PASS\n"); else printf("00772610_TEST FAIL\n");
    return fails;
}
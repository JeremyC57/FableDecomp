// Behaviour test for 0x0059C8F2 CTCInventoryTrade::OnCreate
#include <cstdio>
static int g_destroyed = 0;
struct CDefBase { long m_refCount; void Destroy() { ++g_destroyed; } };
struct CWeaponsDef : CDefBase { long InventoryDefIndex; };
static CWeaponsDef g_sub;
struct CDefClassBase : CDefBase { bool GetSubDef(CWeaponsDef** out) { *out = &g_sub; ++g_sub.m_refCount; return true; } };
struct CThing { CDefClassBase* PDef; };
static int g_registered = 0;
struct CInv {
    CThing* ParentThing; long pdef; bool baseCreated; long CategoryCount, ItemSellCount, ItemBuyCount;
    void SetPDef(long i) { pdef = i; }
    void OnCreate()
    {
        ++g_registered;
        CDefClassBase* thingDef = ParentThing->PDef;
        CWeaponsDef* def = 0;
        if (thingDef) ++thingDef->m_refCount;
        thingDef->GetSubDef(&def);
        if (thingDef && --thingDef->m_refCount == 0) thingDef->Destroy();
        SetPDef(def->InventoryDefIndex);
        baseCreated = true; CategoryCount = ItemSellCount = ItemBuyCount = 0;
        if (--def->m_refCount == 0) def->Destroy();
    }
};
int main()
{
    CDefClassBase owner; owner.m_refCount = 1;
    g_sub.m_refCount = 1; g_sub.InventoryDefIndex = 77;
    CThing t = { &owner }; CInv inv = { &t, 0, false, 5, 6, 7 };
    inv.OnCreate();
    bool ok = inv.pdef == 77 && inv.baseCreated && g_registered == 1 && owner.m_refCount == 1 && g_sub.m_refCount == 1 && g_destroyed == 0 && inv.CategoryCount == 0 && inv.ItemBuyCount == 0;
    if (!ok) { std::printf("0059c8f2_TEST FAIL\n"); return 1; }
    std::printf("0059c8f2_TEST PASS\n");
    return 0;
}

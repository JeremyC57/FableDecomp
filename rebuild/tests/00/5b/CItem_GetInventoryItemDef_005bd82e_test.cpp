// Behaviour test for 005bd82e: body copied from src/005bd82e.cpp (CItem::GetInventoryItemDef), callees stubbed; CItem modelled locally.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CItem { long ItemType; long NumberOfItems; };
static int g_deleted = 0;
struct CIVCountedPointeeBase {
    virtual void Destroy(int flags) {}
    virtual void Delete() { ++g_deleted; }
    unsigned long IVRefCount;
};
struct CInventoryItemDef : CIVCountedPointeeBase { };
struct CInventoryItemDefPointer {
    CInventoryItemDef* Object;
    __forceinline CInventoryItemDefPointer() : Object(0) {}
    __forceinline CInventoryItemDefPointer(const CInventoryItemDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
    __forceinline ~CInventoryItemDefPointer()
    {
        if (Object && --Object->IVRefCount == 0)
            Object->Delete();
    }
};
static CInventoryItemDef* g_itemDef = 0; static int g_getSubDef = 0;
struct CDefClassBase : CIVCountedPointeeBase {
    void GetSubDef(CInventoryItemDefPointer& out);
};
void CDefClassBase::GetSubDef(CInventoryItemDefPointer& out)
{
    ++g_getSubDef;
    if (!g_itemDef) return;
    ++g_itemDef->IVRefCount; out.Object = g_itemDef;
}
struct CDefClassBasePointer {
    CDefClassBase* Object;
    __forceinline CDefClassBasePointer() : Object(0) {}
    __forceinline ~CDefClassBasePointer()
    {
        CDefClassBase* def = Object;
        if (def && --def->IVRefCount == 0)
            def->Delete();
    }
    CDefClassBase* operator->() const { return Object; }
};
static CDefClassBase* g_classDef = 0; static long g_askedIndex = -1; static int g_getDef = 0;
struct CDefinitionManager {
    bool GetDef(long index, CDefClassBasePointer& out);
};
bool CDefinitionManager::GetDef(long index, CDefClassBasePointer& out)
{
    ++g_getDef; g_askedIndex = index;
    if (!g_classDef) return false;
    ++g_classDef->IVRefCount; out.Object = g_classDef; return true;
}
struct CGameComponentDefs { unsigned char _pad00[0x10]; CDefinitionManager* DefinitionManager; };
struct CMainGameComponent { unsigned char _pad00[0x24]; CGameComponentDefs* Defs; };
static CMainGameComponent g_main; CMainGameComponent* g_pMainGameComponent = &g_main;
struct CItem_Methods : CItem {
    CInventoryItemDefPointer GetInventoryItemDef() const;
};
CInventoryItemDefPointer CItem_Methods::GetInventoryItemDef() const
{
    if (NumberOfItems > 0) {
        CDefClassBasePointer def;
        g_pMainGameComponent->Defs->DefinitionManager->GetDef(ItemType, def);
        CInventoryItemDefPointer sub;
        def->GetSubDef(sub);
        return sub;
    }
    return CInventoryItemDefPointer();
}
int main()
{
    static CGameComponentDefs defs; static CDefinitionManager manager; static CDefClassBase classDef; static CInventoryItemDef itemDef;
    g_main.Defs = &defs; defs.DefinitionManager = &manager; classDef.IVRefCount = 1; itemDef.IVRefCount = 1; g_classDef = &classDef; g_itemDef = &itemDef;
    CItem_Methods item; item.ItemType = 17; item.NumberOfItems = 2;
    { CInventoryItemDefPointer r = item.GetInventoryItemDef(); CHECK(r.Object == &itemDef); CHECK(itemDef.IVRefCount == 2); CHECK(g_getDef == 1 && g_askedIndex == 17 && g_getSubDef == 1); CHECK(classDef.IVRefCount == 1); }
    CHECK(itemDef.IVRefCount == 1 && g_deleted == 0);
    item.NumberOfItems = 0;
    { CInventoryItemDefPointer r = item.GetInventoryItemDef(); CHECK(r.Object == 0); CHECK(g_getDef == 1); }
    item.NumberOfItems = 1; classDef.IVRefCount = 0;
    { CInventoryItemDefPointer r = item.GetInventoryItemDef(); CHECK(r.Object == &itemDef); CHECK(g_deleted == 1); }
    if (fails == 0) printf("005bd82e_TEST PASS\n"); else printf("005bd82e_TEST FAIL\n");
    return fails;
}
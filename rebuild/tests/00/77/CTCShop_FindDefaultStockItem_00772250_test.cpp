#include <cstdio>
// Behaviour test for CTCShop::FindDefaultStockItem (0x00772250): the body of src/ over stand-ins -- for a def that is a
// stock item and an inventory item, the shop's default stock record for that def, else the record for its category,
// else null.
struct CShopItem { float Quantity; float PriceMultiplier; float MaxStock; long RestockPeriod; float a, b, c; };
struct CStockItemDef { long Key; CShopItem Item; };
static int g_releases = 0;
struct CDefPointeeBase {
    long m_refCount;
    void ReleaseReference() { --m_refCount; ++g_releases; }
    void ReleaseTemporary() { --m_refCount; ++g_releases; }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->ReleaseReference(); }
    T* operator->() const { return Object; }
};
struct CShopDef : CDefPointeeBase {
    CStockItemDef* DefaultStockFirst;
    CStockItemDef* DefaultStockLast;
    CStockItemDef* CategoryStockFirst;
    CStockItemDef* CategoryStockLast;
};
struct CStockDef : CDefPointeeBase { };
struct CInventoryItemDef : CDefPointeeBase { long Category; };
static CStockDef g_stockDef;
static CInventoryItemDef g_itemDef;
static CShopDef g_shopDef;
struct CDefClassBase : CDefPointeeBase {
    bool isStock, isItem, isShop;
    template <class T> bool Give(CDefPointer<T>* out, T* def, bool has)
    { if (!has) return false; out->Object = def; ++def->m_refCount; return true; }
    bool GetSubDef_0059350c(CDefPointer<CStockDef>* out) { return Give(out, &g_stockDef, isStock); }
    bool GetSubDef_005bade1(CDefPointer<CInventoryItemDef>* out) { return Give(out, &g_itemDef, isItem); }
    bool GetSubDef_005130a0(CDefPointer<CShopDef>* out) { return Give(out, &g_shopDef, isShop); }
};
struct CDefTemporary {
    CDefClassBase* Object;
    CDefTemporary() { }
    CDefTemporary(const CDefTemporary& o) : Object(o.Object) { if (Object) ++Object->m_refCount; }   // holds its own count
    ~CDefTemporary() { if (Object) Object->ReleaseTemporary(); }
    CDefClassBase* operator->() const { return Object; }
};
static CDefClassBase g_queried;
struct CDefinitionManager {
    CDefTemporary GetPDefFromGlobalIndex(long) { CDefTemporary t; t.Object = &g_queried; ++g_queried.m_refCount; return t; }
};
static CDefinitionManager g_defs;
struct CThing { CDefClassBase* PDef; };
struct CTCShop {
    CThing* ParentThing;
    CDefinitionManager* GetDefinitions() const { return &g_defs; }
    const CShopItem* FindDefaultStockItem(long defIndex);
};

const CShopItem* CTCShop::FindDefaultStockItem(long defIndex)
{
    CDefTemporary def = GetDefinitions()->GetPDefFromGlobalIndex(defIndex);
    CDefPointer<CStockDef> stockDef;
    CDefPointer<CInventoryItemDef> itemDef;
    CDefPointer<CShopDef> shopDef;
    if (def->GetSubDef_0059350c(&stockDef) && def->GetSubDef_005bade1(&itemDef)) {
        CDefClassBase* thingDef = ParentThing->PDef;
        if (thingDef->GetSubDef_005130a0(&shopDef)) {
        for (CStockItemDef* it = shopDef->DefaultStockFirst; it != shopDef->DefaultStockLast; ++it)
            if (it->Key == defIndex)
                return &it->Item;
        for (CStockItemDef* it2 = shopDef->CategoryStockFirst; it2 != shopDef->CategoryStockLast; ++it2)
            if (it2->Key == itemDef->Category)
                return &it2->Item;
        }
    }
    return 0;
}

static int fail(int line) { printf("00772250_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) do { if (!(c)) return fail(__LINE__); } while (0)

int main()
{
    CStockItemDef defaults[2] = { { 10, { 1 } }, { 20, { 2 } } };
    CStockItemDef categories[2] = { { 3, { 3 } }, { 4, { 4 } } };
    g_shopDef.DefaultStockFirst = defaults; g_shopDef.DefaultStockLast = defaults + 2;
    g_shopDef.CategoryStockFirst = categories; g_shopDef.CategoryStockLast = categories + 2;
    CDefClassBase shopThingDef; shopThingDef.m_refCount = 1; shopThingDef.isShop = true;
    CThing shopThing = { &shopThingDef };
    CTCShop shop; shop.ParentThing = &shopThing;
    g_queried.m_refCount = 1; g_queried.isStock = true; g_queried.isItem = true;
    g_itemDef.Category = 4;
    CHECK(shop.FindDefaultStockItem(20) == &defaults[1].Item);      // listed by def
    CHECK(shop.FindDefaultStockItem(99) == &categories[1].Item);    // falls back to the category
    g_itemDef.Category = 7;
    CHECK(shop.FindDefaultStockItem(99) == 0);                       // neither
    g_queried.isItem = false;
    CHECK(shop.FindDefaultStockItem(20) == 0);                       // not an inventory item
    g_queried.isItem = true; shopThingDef.isShop = false;
    CHECK(shop.FindDefaultStockItem(20) == 0);                       // the thing is not a shop
    CHECK(g_queried.m_refCount == 1 && g_stockDef.m_refCount == 0 && g_itemDef.m_refCount == 0 && g_shopDef.m_refCount == 0);
    printf("00772250_TEST PASS\n");
    return 0;
}
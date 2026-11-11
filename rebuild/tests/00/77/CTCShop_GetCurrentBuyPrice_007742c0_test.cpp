// Behaviour test for 007742c0 CTCShop::GetCurrentBuyPrice: the row body verbatim over fixture models of the shared-header interfaces
// (CTCBase::GetDefinitionManager, GetDef<CThingBaseDef>, GetSubDef<CShopDef>, counted CDefPointer). Without the shop sub-def the
// price is 0; otherwise count items of the basic price times the def multiplier times the attitude multiplier, truncated per item.
#include <stdio.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct Counted { mutable int Refs; Counted() : Refs(1) { } };
template <class T> class CDefPointer {
    T* Object;
public:
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) --Object->Refs; }
    T* operator->() const { return Object; }
    void Set(T* p) { Object = p; if (p) ++p->Refs; }
};
class CShopDef : public Counted { public: float BuyPriceMultiplier; };
class CThingBaseDef : public Counted { public:
    const CShopDef* Shop;
    CThingBaseDef() : Shop(0) { }
    bool GetSubDef(CDefPointer<const CShopDef>* out) const { if (!Shop) return false; out->Set(Shop); return true; }
};
class CGameDefinitionManager { public:
    const CThingBaseDef* Def; mutable long Index;
    bool GetDef(long index, CDefPointer<const CThingBaseDef>* out) const { Index = index; out->Set(Def); return true; }
};
class CThing { public: unsigned short DefGlobalIndex; unsigned long GetDefGlobalIndex() const { return DefGlobalIndex; } };
const float g_sellPriceScale_013833d4 = 1.0f;
class CTCShop { public:
    CThing* ParentThing; const CGameDefinitionManager* Manager; float Attitude; mutable int BasicCalls; mutable long LastItem;
    const CGameDefinitionManager& GetDefinitionManager() const { return *Manager; }
    float GetAttitudeBuyPriceMultiplier() const { return Attitude; }
    float GetCurrentBasicPrice(long item, long forSale) const { ++BasicCalls; LastItem = item; return forSale ? -1.0f : 10.0f; }
    long GetCurrentBuyPrice(long itemDefIndex, long count) const;
};
// ---- row body (verbatim) ----
long CTCShop::GetCurrentBuyPrice(long itemDefIndex, long count) const
{
    CDefPointer<const CThingBaseDef> thingDef;
    GetDefinitionManager().GetDef(ParentThing->GetDefGlobalIndex(), &thingDef);
    CDefPointer<const CShopDef> shopDef;
    if (!thingDef->GetSubDef(&shopDef))
        return 0;
    float multiplier = shopDef->BuyPriceMultiplier;
    multiplier = multiplier * GetAttitudeBuyPriceMultiplier();
    long total = 0;
    for (long i = count; i > 0; --i)
        total += (long)(GetCurrentBasicPrice(itemDefIndex, 0) * multiplier);
    return total;
}
// ---- harness ----
int main()
{
    CShopDef shopDef; CThingBaseDef thingDef; CGameDefinitionManager manager; CThing thing; CTCShop shop;
    thing.DefGlobalIndex = 77; manager.Def = &thingDef; thingDef.Shop = &shopDef;
    shop.ParentThing = &thing; shop.Manager = &manager; shop.Attitude = 1.5f; shop.BasicCalls = 0; shop.LastItem = 0;
    shopDef.BuyPriceMultiplier = 1.25f;                         // 10 * 1.25 * 1.5 = 18.75
    long each = 18;
    CHECK(shop.GetCurrentBuyPrice(5, 3) == 3 * each && shop.BasicCalls == 3 && shop.LastItem == 5 && manager.Index == 77);
    CHECK(shop.GetCurrentBuyPrice(5, 0) == 0 && shop.BasicCalls == 3);
    CHECK(thingDef.Refs == 1 && shopDef.Refs == 1);
    thingDef.Shop = 0;
    CHECK(shop.GetCurrentBuyPrice(5, 3) == 0 && shop.BasicCalls == 3 && thingDef.Refs == 1);
    if (fails == 0) printf("007742c0_TEST PASS\n");
    return fails;
}

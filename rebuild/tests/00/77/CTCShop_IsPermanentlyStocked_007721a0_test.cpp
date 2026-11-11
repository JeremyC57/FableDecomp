// Behaviour test for 007721a0 CTCShop::IsPermanentlyStocked: the row body verbatim over fixture models of the shared-header
// interfaces it uses (CTCBase::GetDefinitionManager, CDefinitionManager::GetDef<CThingBaseDef>, CDefClassBase::GetSubDef<CShopDef>,
// counted CDefPointer). True when the shop sub-def has IsStockDisplayPermanent || AlwaysOpen, false without the sub-def; both
// defs released.
#include <stdio.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct Counted { mutable int Refs; mutable int Releases; Counted() : Refs(1), Releases(0) { } };
template <class T> class CDefPointer {
    T* Object;
public:
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { --Object->Refs; ++Object->Releases; } }
    T* operator->() const { return Object; }
    void Set(T* p) { Object = p; if (p) ++p->Refs; }
};
class CShopDef : public Counted { public: bool IsStockDisplayPermanent; bool AlwaysOpen; };
class CThingBaseDef : public Counted { public:
    const CShopDef* Shop; mutable int Queries;
    CThingBaseDef() : Shop(0), Queries(0) { }
    bool GetSubDef(CDefPointer<const CShopDef>* out) const { ++Queries; if (!Shop) return false; out->Set(Shop); return true; }
};
class CGameDefinitionManager { public:
    const CThingBaseDef* Def; mutable long Index;
    CGameDefinitionManager() : Def(0), Index(-1) { }
    bool GetDef(long index, CDefPointer<const CThingBaseDef>* out) const { Index = index; out->Set(Def); return Def != 0; }
};
class CThing { public: unsigned short DefGlobalIndex; unsigned long GetDefGlobalIndex() const { return DefGlobalIndex; } };
class CTCShop { public:
    CThing* ParentThing; const CGameDefinitionManager* Manager;
    const CGameDefinitionManager& GetDefinitionManager() const { return *Manager; }
    bool IsPermanentlyStocked() const;
};
// ---- row body (verbatim) ----
bool CTCShop::IsPermanentlyStocked() const
{
    CDefPointer<const CThingBaseDef> thingDef;
    GetDefinitionManager().GetDef(ParentThing->GetDefGlobalIndex(), &thingDef);
    CDefPointer<const CShopDef> shopDef;
    if (thingDef->GetSubDef(&shopDef))
        return shopDef->IsStockDisplayPermanent || shopDef->AlwaysOpen;
    return false;
}
// ---- harness ----
int main()
{
    CShopDef shopDef; CThingBaseDef thingDef; CGameDefinitionManager manager; CThing thing; CTCShop shop;
    thing.DefGlobalIndex = 0x1234; manager.Def = &thingDef; thingDef.Shop = &shopDef;
    shop.ParentThing = &thing; shop.Manager = &manager;
    shopDef.IsStockDisplayPermanent = true; shopDef.AlwaysOpen = false;
    CHECK(shop.IsPermanentlyStocked() && manager.Index == 0x1234);
    shopDef.IsStockDisplayPermanent = false; shopDef.AlwaysOpen = true;
    CHECK(shop.IsPermanentlyStocked());
    shopDef.AlwaysOpen = false;
    CHECK(!shop.IsPermanentlyStocked());
    CHECK(thingDef.Refs == 1 && shopDef.Refs == 1 && thingDef.Releases == 3 && shopDef.Releases == 3);
    thingDef.Shop = 0;
    CHECK(!shop.IsPermanentlyStocked() && thingDef.Refs == 1 && thingDef.Releases == 4 && shopDef.Releases == 3);
    CHECK(thingDef.Queries == 4);
    if (fails == 0) printf("007721a0_TEST PASS\n");
    return fails;
}

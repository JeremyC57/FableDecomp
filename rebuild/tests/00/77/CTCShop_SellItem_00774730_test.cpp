// Behaviour test for 0x00774730 CTCShop::SellItem(long): the row's logic over stub classes (C++03).
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <list>
static void check(bool v, int line) { if (!v) { printf("00774730_TEST FAIL line %d\n", line); exit(1); } }
class CThing { public:
    bool dead; unsigned short DefGlobalIndex; int kills;
    CThing(unsigned short def, bool d) : dead(d), DefGlobalIndex(def), kills(0) { }
    bool IsDead() const { return dead; }
    unsigned long GetDefGlobalIndex() const { return DefGlobalIndex; }
    void Kill(bool immediately) { check(immediately, __LINE__); ++kills; }
};
template <class T> class CIntelligentPointer { public:
    T* p; CIntelligentPointer(T* t) : p(t) { }
    T* operator->() const { return p; }
};
template <class T> bool operator!=(const CIntelligentPointer<T>& a, int) { return a.p != 0; }
template <class T> bool IS_THING_ALIVE(const T& thing) { if (thing != 0) { if (!thing->IsDead()) return true; } return false; }
class CShopDef { public: float MaxStockChangePerSell; };
template <class T> class CDefPointer { public: const T* Object; CDefPointer() : Object(0) { } const T* operator->() const { return Object; } };
class CShopItem { public: float Quantity, PriceMultiplier, MaxStock; };
class CTCThingOwner { public:
    std::list<CIntelligentPointer<CThing> > ObjectList;
    const std::list<CIntelligentPointer<CThing> >& GetObjectList() const { return ObjectList; }
};
class CThingParent { public:
    CShopDef* def; CTCThingOwner* owner;
    const CThingParent& PeekBaseDef() const { return *this; }
    bool GetSubDef(CDefPointer<const CShopDef>* out) const { out->Object = def; return true; }
    bool PeekTC(const CTCThingOwner** out) const { if (!owner) return false; *out = owner; return true; }
};
class CTCShop { public:
    CThingParent parent;
    std::map<long, CShopItem> Stock;
    long SpecialItem, GoldDonated;
    const CThingParent& GetParentThing() const { return parent; }
    void SellItem(long item);
};

void CTCShop::SellItem(long item)
{
    CDefPointer<const CShopDef> shop_def;
    GetParentThing().PeekBaseDef().GetSubDef(&shop_def);
    if (item == SpecialItem) {
        SpecialItem = -1;
        GoldDonated = 0;
    }
    if (Stock.find(item) == Stock.end())
        return;
    float quantity = Stock[item].Quantity;
    Stock[item].Quantity = quantity < 1.0f ? 0.0f : quantity - 1.0f;
    Stock[item].MaxStock += shop_def->MaxStockChangePerSell;
    if (Stock[item].MaxStock < 0.0f)
        Stock[item].MaxStock = 0.0f;
    const CTCThingOwner* owner;
    if (GetParentThing().PeekTC(&owner)) {
        for (std::list<CIntelligentPointer<CThing> >::const_iterator it = owner->GetObjectList().begin(); it != owner->GetObjectList().end(); ++it) {
            const CIntelligentPointer<CThing>& thing = *it;
            if (IS_THING_ALIVE(thing) && thing->GetDefGlobalIndex() == item) {
                thing->Kill(true);
                break;
            }
        }
    }
}

int main()
{
    CShopDef def; def.MaxStockChangePerSell = -2.0f;
    CTCThingOwner owner;
    CThing dead7(7, true), live9(9, false), live7a(7, false), live7b(7, false);
    owner.ObjectList.push_back(CIntelligentPointer<CThing>(0));
    owner.ObjectList.push_back(CIntelligentPointer<CThing>(&dead7));
    owner.ObjectList.push_back(CIntelligentPointer<CThing>(&live9));
    owner.ObjectList.push_back(CIntelligentPointer<CThing>(&live7a));
    owner.ObjectList.push_back(CIntelligentPointer<CThing>(&live7b));
    CTCShop shop; shop.parent.def = &def; shop.parent.owner = &owner; shop.SpecialItem = 7; shop.GoldDonated = 50;
    CShopItem it7 = { 3.5f, 1.0f, 1.5f }; shop.Stock[7] = it7;
    shop.SellItem(7);
    check(shop.SpecialItem == -1 && shop.GoldDonated == 0, __LINE__);
    check(shop.Stock[7].Quantity == 2.5f && shop.Stock[7].MaxStock == 0.0f, __LINE__);   // 1.5 - 2 clamped to 0
    check(dead7.kills == 0 && live9.kills == 0 && live7a.kills == 1 && live7b.kills == 0, __LINE__);
    shop.Stock[7].Quantity = 0.5f; shop.Stock[7].MaxStock = 5.0f;
    shop.SellItem(7);
    check(shop.Stock[7].Quantity == 0.0f && shop.Stock[7].MaxStock == 3.0f, __LINE__);
    check(live7a.kills == 2, __LINE__);
    shop.SpecialItem = 4; shop.GoldDonated = 10;
    shop.SellItem(11);                                     // not stocked: only the special-item test runs
    check(shop.SpecialItem == 4 && shop.GoldDonated == 10 && shop.Stock.size() == 1, __LINE__);
    shop.parent.owner = 0; shop.SellItem(7);               // no thing owner: no kill
    check(live7a.kills == 2 && shop.Stock[7].MaxStock == 1.0f, __LINE__);
    printf("00774730_TEST PASS\n");
    return 0;
}
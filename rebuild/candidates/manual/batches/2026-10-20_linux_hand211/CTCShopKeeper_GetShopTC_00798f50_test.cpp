// Behaviour test for CTCShopKeeper::GetShopTC (0x00798F50): an alive shop thing's interface 0x53 wins; a dead or missing shop
// thing falls back to the keeper's own thing; null when the chosen thing has no shop interface.
#include <cstdio>
#include <cstring>
struct CThing;
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags91;
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00() { }
    void* m_pItem;
    CThing* GetPItem() const { return (CThing*)m_pItem; }
};
struct CTCShop { int id; };
struct CTCShopKeeper {
    virtual void v00() { }
    CThing* ParentThing;
    bool ToBeRemoved;
    bool IsInGlobalUpdateTCs;
    unsigned char _pad0a[2];
    CBaseIntelligentPointer Customer;
    CBaseIntelligentPointer Shop;
    CTCShop* GetShopTC() const;
};

CTCShop* CTCShopKeeper::GetShopTC() const
{
    CTCShop* shopTC = 0;
    if (Shop.GetPItem() && !(Shop.GetPItem()->m_flags91 & 1)) {
        CThing* shop = Shop.GetPItem();
        if (shop->HasTC(0x53))
            shopTC = (CTCShop*)shop->GetTC(0x53);
    } else {
        if (ParentThing->HasTC(0x53))
            shopTC = (CTCShop*)ParentThing->GetTC(0x53);
    }
    return shopTC;
}

static void Make(CThing& t, CInterfaceMap::SEntry* e, CTCShop* shop)
{
    std::memset(t.m_presentBits, 0, sizeof(t.m_presentBits)); t.m_flags91 = 0;
    e[0].type = 0x10; e[0].component = 0; e[1].type = 0x53; e[1].component = (CTCBase*)shop;
    t.m_interfaces.first = e; t.m_interfaces.last = e + 2;
    if (shop) t.m_presentBits[0x53 >> 5] = 1UL << (0x53 & 31);
}

int main()
{
    int bad = 0;
    CTCShop a, b; a.id = 1; b.id = 2;
    CInterfaceMap::SEntry ea[2], eb[2];
    CThing shopThing, keeperThing; Make(shopThing, ea, &a); Make(keeperThing, eb, &b);
    CTCShopKeeper k; k.ParentThing = &keeperThing; k.Shop.m_pItem = &shopThing; k.Customer.m_pItem = 0;
    if (k.GetShopTC() != &a) bad |= 1;
    shopThing.m_flags91 = 1;
    if (k.GetShopTC() != &b) bad |= 2;                 // dead shop thing: the keeper's own
    shopThing.m_flags91 = 0; k.Shop.m_pItem = 0;
    if (k.GetShopTC() != &b) bad |= 4;
    Make(keeperThing, eb, 0);
    if (k.GetShopTC() != 0) bad |= 8;
    k.Shop.m_pItem = &shopThing; Make(shopThing, ea, 0);
    if (k.GetShopTC() != 0) bad |= 16;                 // alive shop thing without the interface: no fallback
    if (bad) { std::printf("00798f50_TEST FAIL\n"); return 1; }
    std::printf("00798f50_TEST PASS\n");
    return 0;
}

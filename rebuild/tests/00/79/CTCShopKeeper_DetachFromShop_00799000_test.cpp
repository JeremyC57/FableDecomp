// Behaviour test for CTCShopKeeper::DetachFromShop (0x00799000): an alive shop thing with a shop interface gets its keeper
// cleared and the GUI refreshed with 0; the keeper's Shop pointer is always cleared; no refresh otherwise.
#include <cstdio>
#include <cstring>
class CThing;
class CBaseObject;
class CThingCreatureBase;
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
class CThing {
public:
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
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    ~CBaseIntelligentPointer() { }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CBaseObject* item) { m_pItem = (CThing*)item; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00() { }
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};
class CTCShop { public:
    int calls; CThingCreatureBase* keeper;
    void SetShopKeeper(CThingCreatureBase* shopKeeper) { ++calls; keeper = shopKeeper; }
};
class CPlayerGui { public:
    int calls; long last;
    void Refresh(long flag) { ++calls; last = flag; }
};
CPlayerGui* g_pPlayerGui;
class CTCShopKeeper { public:
    virtual void v00() { }
    CThing* ParentThing;
    bool ToBeRemoved;
    bool IsInGlobalUpdateTCs;
    unsigned char _pad0a[2];
    CBaseIntelligentPointer Customer;
    CBaseIntelligentPointer Shop;
    void DetachFromShop();
};

void CTCShopKeeper::DetachFromShop()
{
    bool detached = false;
    if (Shop.GetPItem() && !(Shop.GetPItem()->m_flags91 & 1)) {
        CThing* shop = Shop.GetPItem();
        if (shop->HasTC(0x53)) {
            ((CTCShop*)shop->GetTC(0x53))->SetShopKeeper(0);
            detached = true;
        }
    }
    {
        CIntelligentPointer<CThing> cleared(0);
        CThing* item = cleared.GetPItem();
        Shop.SetPItem((CBaseObject*)item);
    }
    if (detached)
        g_pPlayerGui->Refresh(0);
}

int main()
{
    int bad = 0;
    CTCShop shopTC; shopTC.calls = 0; shopTC.keeper = (CThingCreatureBase*)&shopTC;
    CInterfaceMap::SEntry e[1] = { { 0x53, (CTCBase*)&shopTC } };
    CThing shop; std::memset(shop.m_presentBits, 0, sizeof(shop.m_presentBits)); shop.m_flags91 = 0;
    shop.m_presentBits[0x53 >> 5] = 1UL << (0x53 & 31); shop.m_interfaces.first = e; shop.m_interfaces.last = e + 1;
    CPlayerGui gui; gui.calls = 0; gui.last = -1; g_pPlayerGui = &gui;
    CTCShopKeeper k; k.Shop.SetPItem((CBaseObject*)&shop);
    k.DetachFromShop();
    if (shopTC.calls != 1 || shopTC.keeper != 0 || k.Shop.GetPItem() != 0 || gui.calls != 1 || gui.last != 0) bad |= 1;
    k.DetachFromShop();                                   // no shop any more
    if (shopTC.calls != 1 || gui.calls != 1) bad |= 2;
    shop.m_flags91 = 1; k.Shop.SetPItem((CBaseObject*)&shop);
    k.DetachFromShop();                                   // dead shop: cleared, not told
    if (shopTC.calls != 1 || gui.calls != 1 || k.Shop.GetPItem() != 0) bad |= 4;
    shop.m_flags91 = 0; shop.m_presentBits[0x53 >> 5] = 0; k.Shop.SetPItem((CBaseObject*)&shop);
    k.DetachFromShop();                                   // no shop interface
    if (shopTC.calls != 1 || gui.calls != 1 || k.Shop.GetPItem() != 0) bad |= 8;
    if (bad) { std::printf("00799000_TEST FAIL\n"); return 1; }
    std::printf("00799000_TEST PASS\n");
    return 0;
}
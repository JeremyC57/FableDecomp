// Behaviour test for 00773e10: body copied from src/00773e10.cpp (CTCShop::RemoveStockItemsFromShop), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CThing;
struct CTCBase;
static int g_destroyed = 0;
struct CDefBase {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long RefCount;
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->Release(); }
};
struct CStockItemDef : CDefBase { };
static CStockItemDef g_stockDef; static int g_getSubDef = 0;
struct CDefClassBase : CDefBase {
    bool IsStock;
    bool GetSubDef_0059350c(CDefPointer<CStockItemDef>& out) { ++g_getSubDef; if (!IsStock) return false; ++g_stockDef.RefCount; out.Object = &g_stockDef; return true; }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);
};
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
static int g_kills = 0; static CThing* g_killed[4];
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* PDef;
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
    void Kill(bool immediately) { CHECK(immediately); if (g_kills < 4) g_killed[g_kills] = this; ++g_kills; }
};
struct CBaseIntelligentPointer {
    virtual void v00() {}
    void* m_pItem;
    CThing* GetPItem() const { return (CThing*)m_pItem; }
};
struct CObjectNode {
    CObjectNode* _M_next;
    CObjectNode* _M_prev;
    CBaseIntelligentPointer _M_data;
};
struct CTCThingOwner {
    virtual void v00() {}
    CThing* ParentThing;
    bool ToBeRemoved;
    bool IsInGlobalUpdateTCs;
    unsigned char _pad0a[2];
    CObjectNode* ObjectList;
};
static int g_stockState = -1; static int g_setStock = 0;
struct CTCShop {
    virtual void v00() {}
    CThing* ParentThing;
    void SetStockState(long state) { ++g_setStock; g_stockState = state; }
    void RemoveStockItemsFromShop();
};
void CTCShop::RemoveStockItemsFromShop()
{
    CThing* shop = ParentThing;
    if (shop->HasTC(0x21)) {
        CTCThingOwner* owner = (CTCThingOwner*)shop->GetTC(0x21);
        for (CObjectNode* node = owner->ObjectList->_M_next; node != owner->ObjectList; node = node->_M_next) {
            CBaseIntelligentPointer& object = node->_M_data;
            CDefPointer<CStockItemDef> stockDef;
            if (object.GetPItem() && !(object.GetPItem()->m_flags91 & 1)) {
                CDefClassBase* def = object.GetPItem()->PDef;
                if (def->GetSubDef_0059350c(stockDef))
                    object.GetPItem()->Kill(true);
            }
        }
    }
    SetStockState(1);
}
int main()
{
    static CThing shop, a, b, c, d; static CDefClassBase stockClass, plainClass; static CTCThingOwner owner;
    memset(&shop, 0, sizeof(shop)); memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b)); memset(&c, 0, sizeof(c)); memset(&d, 0, sizeof(d));
    stockClass.IsStock = true; plainClass.IsStock = false; g_stockDef.RefCount = 1;
    a.PDef = &stockClass; b.PDef = &plainClass; c.PDef = &stockClass; c.m_flags91 = 1; d.PDef = &stockClass;
    CObjectNode header, n1, n2, n3, n4, n5;
    header._M_next = &n1; n1._M_next = &n2; n2._M_next = &n3; n3._M_next = &n4; n4._M_next = &n5; n5._M_next = &header;
    n1._M_data.m_pItem = &a; n2._M_data.m_pItem = &b; n3._M_data.m_pItem = &c; n4._M_data.m_pItem = 0; n5._M_data.m_pItem = &d;
    owner.ObjectList = &header;
    CInterfaceMap::SEntry entries[2] = { { 0x05, 0 }, { 0x21, (CTCBase*)&owner } }; shop.m_interfaces.first = entries; shop.m_interfaces.last = entries + 2;
    CTCShop s; s.ParentThing = &shop;
    // no thing-owner component: only the stock state changes
    s.RemoveStockItemsFromShop(); CHECK(g_kills == 0 && g_setStock == 1 && g_stockState == 1);
    // with the owner: live things with a stock-item def are killed (a and d); b has no stock def, c is dead, slot 4 is empty
    shop.m_presentBits[0x21 >> 5] |= 1UL << (0x21 & 31); s.RemoveStockItemsFromShop();
    CHECK(g_kills == 2 && g_killed[0] == &a && g_killed[1] == &d); CHECK(g_getSubDef == 3 && g_setStock == 2); CHECK(g_stockDef.RefCount == 1 && g_destroyed == 0);
    // a stock def held only by the lookup is destroyed after each use
    g_stockDef.RefCount = 0; s.RemoveStockItemsFromShop(); CHECK(g_destroyed >= 1 && g_kills == 4);
    if (fails == 0) printf("00773e10_TEST PASS\n"); else printf("00773e10_TEST FAIL\n");
    return fails;
}
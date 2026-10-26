// Behaviour test for 00773aa0: body copied from src/00773aa0.cpp (CTCShop::IsShopKeeperAvailable), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CTCBaseAny;
struct CInterfaceMap {
    struct SEntry { long type; CTCBaseAny* component; };
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
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> bool GetInterface(T** ppOut, long type)
    {
        if (HasInterface(type)) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00() {}
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
class CTCShopKeeper { public:
    unsigned char _pad00[0x42];
    bool RepressedOpinions;
};
class CTCShop { public:
    virtual void v00() {}
    void*                   ParentThing;
    bool                    ToBeRemoved;
    bool                    IsInGlobalUpdateTCs;
    long                    StockState;
    CBaseIntelligentPointer PShopKeeper;
    long IsShopKeeperAvailable() const;
};
long CTCShop::IsShopKeeperAvailable() const
{
    if (StockState == 0 && IS_THING_ALIVE(PShopKeeper)) {
        CThing* keeper = PShopKeeper.GetPItem();
        CTCShopKeeper* shopKeeper;
        if (keeper->GetInterface(&shopKeeper, 0x90)) {
            if (shopKeeper->RepressedOpinions)
                return 1;
        }
    }
    return 0;
}
int main()
{
    static CThing keeper; static CTCShopKeeper shopKeeper; memset(&keeper, 0, sizeof(keeper)); shopKeeper.RepressedOpinions = true;
    CInterfaceMap::SEntry entries[2] = { { 0x21, 0 }, { 0x90, (CTCBaseAny*)&shopKeeper } }; keeper.m_interfaces.first = entries; keeper.m_interfaces.last = entries + 2;
    CTCShop shop; shop.StockState = 0; shop.PShopKeeper.m_pItem = 0;
    // no keeper: 0
    CHECK(shop.IsShopKeeperAvailable() == 0);
    // keeper without the shop-keeper component: 0
    shop.PShopKeeper.m_pItem = &keeper; CHECK(shop.IsShopKeeperAvailable() == 0);
    // component present with repressed opinions: 1
    keeper.m_presentBits[0x90 >> 5] |= 1UL << (0x90 & 31); CHECK(shop.IsShopKeeperAvailable() == 1);
    // opinions not repressed: 0
    shopKeeper.RepressedOpinions = false; CHECK(shop.IsShopKeeperAvailable() == 0); shopKeeper.RepressedOpinions = true;
    // dead keeper: 0; stock state other than 0: 0
    keeper.m_flags91 = 1; CHECK(shop.IsShopKeeperAvailable() == 0); keeper.m_flags91 = 0;
    shop.StockState = 2; CHECK(shop.IsShopKeeperAvailable() == 0);
    if (fails == 0) printf("00773aa0_TEST PASS\n"); else printf("00773aa0_TEST FAIL\n");
    return fails;
}
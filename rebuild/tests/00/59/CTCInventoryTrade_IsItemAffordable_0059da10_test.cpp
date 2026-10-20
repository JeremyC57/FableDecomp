// Behaviour test for 0059da10: the model below is a copy of src/0059da10.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#endif
static int g_priceCalls = 0, g_lastItem = 0, g_lastCount = 0, g_price = 0, g_lowerCalls = 0;
struct CPresenceBits {
    bool test(long bit);
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type);
};
struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            CInterfaceMap::SEntry* found = (it == map->last || wanted < it->type) ? map->last : it;
            *out = (T*)found->component;
            return true;
        }
        return false;
    }
};
struct CTCShop {                                     // interface type 0x53
    long GetCurrentBuyPrice(long itemType, long count) { ++g_priceCalls; g_lastItem = itemType; g_lastCount = count; return g_price; }
};
struct CTCGoldOwner {                                // interface type 4: the hero's purse
    unsigned char _pad00[0x3c];
    long Gold;                                       // +0x3c
};
static CThing* g_hero = 0;
struct CPlayer {
    CThing* GetHeroThing() { return g_hero; }
};
struct CPlayerManager {
    CPlayer* GetMainPlayer() const { static CPlayer p; return &p; }
};
struct CMainGameComponent {
    unsigned char   _pad00[0x1c];
    CPlayerManager* PlayerManager;                   // +0x1c
};
CMainGameComponent* g_pMainGameComponent;

static CThing* g_trader = 0;
namespace NInventory {
struct CTCInventoryTrade {
    CThing* GetTradingThing() { return g_trader; }
    bool IsItemAffordable(long itemType, long count);
};
bool CTCInventoryTrade::IsItemAffordable(long itemType, long count)
{
    CTCShop* shop;
    GetTradingThing()->GetInterface(&shop, 0x53);
    long price = shop->GetCurrentBuyPrice(itemType, count);
    CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetHeroThing();
    CTCGoldOwner* purse = 0;
    hero->GetInterface(&purse, 4);
    bool affordable = purse->Gold >= price;
    return affordable;
}
}

// ---- test doubles and driver ----
bool CPresenceBits::test(long bit) { return true; }
// each thing carries one two-entry sorted map: {4, purse} {0x53, shop}
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    ++g_lowerCalls;
    for (SEntry* e = first; e != last; ++e)
        if (e->type >= *type) return e;
    return last;
}
static void Setup(CThing& t, CInterfaceMap::SEntry* entries, int n)
{
    std::memset(&t, 0, sizeof(t));
    t.m_interfaces.first = entries; t.m_interfaces.last = entries + n;
}
int main()
{
    CMainGameComponent mgc; CPlayerManager pm; mgc.PlayerManager = &pm; g_pMainGameComponent = &mgc;
    CTCShop shop; CTCGoldOwner purse; std::memset(&purse, 0, sizeof(purse));
    CThing trader, hero;
    CInterfaceMap::SEntry traderMap[2] = { {4, 0}, {0x53, &shop} };
    CInterfaceMap::SEntry heroMap[2]   = { {4, &purse}, {0x53, 0} };
    Setup(trader, traderMap, 2); Setup(hero, heroMap, 2);
    g_trader = &trader; g_hero = &hero;
    NInventory::CTCInventoryTrade inv;
    bool ok = true;
    g_price = 100; purse.Gold = 100; ok = ok && inv.IsItemAffordable(7, 2) == true && g_lastItem == 7 && g_lastCount == 2;
    g_price = 101; purse.Gold = 100; ok = ok && inv.IsItemAffordable(7, 1) == false;
    g_price = 0;   purse.Gold = 0;   ok = ok && inv.IsItemAffordable(3, 5) == true;
    g_price = -5;  purse.Gold = -10; ok = ok && inv.IsItemAffordable(3, 5) == false;   // signed compare
    g_price = 50;  purse.Gold = 1000; ok = ok && inv.IsItemAffordable(9, 9) == true && g_priceCalls == 5 && g_lowerCalls == 10;
    if (!ok) { std::printf("0059da10_TEST FAIL\n"); return 1; }
    std::printf("0059da10_TEST PASS\n");
    return 0;
}
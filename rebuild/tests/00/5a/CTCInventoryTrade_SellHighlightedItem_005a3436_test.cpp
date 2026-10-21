#include <cstdio>
#include <cstdlib>
// Behaviour test for CTCInventoryTrade::SellHighlightedItem (0x005A3436): same body as src/, with the category, owner
// and hero lookups, the shop, the hero stats and the list rebuilds stubbed and recorded.
static int g_order = 0, g_resetAt = 0, g_buyAt = 0, g_sellInitAt = 0, g_animAt = 0, g_ownedAt = 0;
static long g_soldItem = 0, g_paid = 0, g_removedItem = 0, g_removedCount = 0, g_animItem = 0, g_ownedItem = 0;
static bool g_paidShow = false;
static int g_sales = 0, g_payments = 0, g_removes = 0;

class CTCBase { };
struct CTCShop : CTCBase {
    long priceEach;
    long GetCurrentBuyPrice(long defIndex, long count) const { return priceEach * count + defIndex; }
    void SellHighlightedItem(long defIndex) { ++g_sales; g_soldItem = defIndex; }
};
struct CTCHeroStats : CTCBase {
    void AddMoney(long amount, bool showMessage) { ++g_payments; g_paid = amount; g_paidShow = showMessage; }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};
class CThing {
public:
    unsigned long presentMask;
    CInterfaceMap m_interfaces;
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (presentMask & (1ul << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
static CThing* g_hero = 0;
struct CPlayer { CThing* GetHeroCreature() { return g_hero; } };
struct CPlayerManager { CPlayer player; CPlayer* GetMainPlayer() { return &player; } };
struct CEngine { CPlayerManager* PlayerManager; };
static CEngine* g_pEngine = 0;

namespace NInventory {
struct CCategory {
    long HighlightedIndex;
    long slots[4];
    long GetItemInSlotWithIndex(long index) const { return slots[index]; }
};
}
static int g_vecFrees = 0;
struct CPairVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    CPairVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CPairVector() { if (_M_start) { ++g_vecFrees; std::free(_M_start); } }
};
struct CTCInventoryBase {
    CThing* owner;
    NInventory::CCategory* selected;
    long highlighted;
    CThing* GetOwner() const { return owner; }
    NInventory::CCategory* GetSelectedCategory() { return selected; }
    long GetHighlightedItem() const { return highlighted; }
    void ResetInventoryAnim(long item) { g_animItem = item; g_animAt = ++g_order; }
    void SetNumberOfSelectedItemOwned(long item) { g_ownedItem = item; g_ownedAt = ++g_order; }
    virtual void RemoveItem(long item, long count) { ++g_removes; g_removedItem = item; g_removedCount = count; }
    virtual ~CTCInventoryBase() { }
};
struct CTCInventoryTrade : CTCInventoryBase {
    long CategoryIdent;
    void ResetTradeLists() { g_resetAt = ++g_order; }
    void InitialiseBuyableItems(CPairVector& items)
    {
        g_buyAt = ++g_order;
        items._M_start = (long*)std::malloc(8); items._M_finish = items._M_start + 2; items._M_end_of_storage = items._M_finish;
    }
    void InitialiseSellableItems() { g_sellInitAt = ++g_order; }
    void SellHighlightedItem();
};

void CTCInventoryTrade::SellHighlightedItem()
{
    CategoryIdent = 0xc;
    NInventory::CCategory* category = GetSelectedCategory();
    if (category) {
        long item = category->GetItemInSlotWithIndex(category->HighlightedIndex);
        if (item > 0) {
            CTCShop* shop = 0;
            GetOwner()->GetInterface(&shop, 0x53);
            long price = shop->GetCurrentBuyPrice(item, 1);
            shop->SellHighlightedItem(item);
            CThing* hero = g_pEngine->PlayerManager->GetMainPlayer()->GetHeroCreature();
            CTCHeroStats* stats = 0;
            hero->GetInterface(&stats, 4);
            stats->AddMoney(price, true);
            RemoveItem(item, 1);
        }
    }
    ResetTradeLists();
    CPairVector buyable;
    InitialiseBuyableItems(buyable);
    InitialiseSellableItems();
    ResetInventoryAnim(GetHighlightedItem());
    SetNumberOfSelectedItemOwned(GetHighlightedItem());
}

int main()
{
    int fails = 0;
    CTCShop shop; shop.priceEach = 100;
    CTCHeroStats stats;
    CInterfaceMap::SEntry ownerEntries[3] = { { 0x10, 0 }, { 0x53, &shop }, { 0x60, 0 } };
    CThing owner; owner.presentMask = 1ul << (0x53 & 31); owner.m_interfaces.first = ownerEntries; owner.m_interfaces.last = ownerEntries + 3;
    CInterfaceMap::SEntry heroEntries[2] = { { 4, &stats }, { 9, 0 } };
    CThing hero; hero.presentMask = 1ul << 4; hero.m_interfaces.first = heroEntries; hero.m_interfaces.last = heroEntries + 2;
    g_hero = &hero;
    CPlayerManager pm; CEngine engine; engine.PlayerManager = &pm; g_pEngine = &engine;
    NInventory::CCategory cat; cat.HighlightedIndex = 2; cat.slots[0] = 5; cat.slots[1] = 0; cat.slots[2] = 42; cat.slots[3] = -1;
    CTCInventoryTrade tc; tc.owner = &owner; tc.selected = &cat; tc.highlighted = 77; tc.CategoryIdent = 0;
    tc.SellHighlightedItem();
    if (tc.CategoryIdent != 0xc || g_sales != 1 || g_soldItem != 42) ++fails;
    if (g_payments != 1 || g_paid != 142 || !g_paidShow) ++fails;
    if (g_removes != 1 || g_removedItem != 42 || g_removedCount != 1) ++fails;
    if (!(g_resetAt < g_buyAt && g_buyAt < g_sellInitAt && g_sellInitAt < g_animAt && g_animAt < g_ownedAt)) ++fails;
    if (g_animItem != 77 || g_ownedItem != 77 || g_vecFrees != 1) ++fails;
    // Empty slot: nothing sold, lists still rebuilt.
    cat.HighlightedIndex = 1;
    tc.SellHighlightedItem();
    if (g_sales != 1 || g_payments != 1 || g_removes != 1 || g_vecFrees != 2 || g_ownedAt != 10) ++fails;
    // No category selected: lists still rebuilt.
    tc.selected = 0; tc.CategoryIdent = 3;
    tc.SellHighlightedItem();
    if (tc.CategoryIdent != 0xc || g_sales != 1 || g_vecFrees != 3 || g_ownedAt != 15) ++fails;
    if (fails) { std::printf("005a3436_TEST FAIL %d\n", fails); return 1; }
    std::printf("005a3436_TEST PASS\n");
    return 0;
}
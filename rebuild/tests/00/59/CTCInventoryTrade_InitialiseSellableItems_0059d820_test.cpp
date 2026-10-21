#include <cstdio>
#include <cstdlib>
#include <vector>
// Behaviour test for CTCInventoryTrade::InitialiseSellableItems (0x0059D820): same body as src/, with the player
// lookup, interface map, item gathering and the trade helpers stubbed and recorded.
class CTCBase;
struct CPresenceBits {
    unsigned long bits[8];
    bool test(long kind) const { return (bits[kind >> 5] >> (kind & 31)) & 1; }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const { const SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
class CThing {
public:
    CPresenceBits present;
    CInterfaceMap m_interfaces;
    bool HasTC(long type) const { return present.test(type); }
    template<class T> bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (present.test(interfaceType)) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
static CThing* g_hero = 0;
struct CPlayer { CThing* GetControlledCreature() const { return g_hero; } };
static CPlayer g_player;
struct CPlayerManager { CPlayer* GetMainPlayer() const { return &g_player; } };
static CPlayerManager g_pm;
struct CEngine { CPlayerManager* PlayerManager; };
static CEngine g_engine = { &g_pm };
static CEngine* g_pEngine = &g_engine;
namespace NInventory {
struct CItem { long DefIndex; long Count; };
struct CCategory { long unique; long GetUniqueItemCountInCategory() const { return unique; } };
}
struct CItemVector {
    NInventory::CItem* _M_start;
    NInventory::CItem* _M_finish;
    NInventory::CItem* _M_end_of_storage;
    CItemVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CItemVector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return _M_finish - _M_start; }
    NInventory::CItem& operator[](unsigned long n) { return _M_start[n]; }
    void push_back(const NInventory::CItem& x)
    {
        unsigned long n = size();
        _M_start = (NInventory::CItem*)realloc(_M_start, (n + 1) * sizeof x);
        _M_start[n] = x; _M_finish = _M_start + n + 1; _M_end_of_storage = _M_finish;
    }
};
struct CTCInventoryItem { static bool IsBuyable(long defIndex) { return defIndex != 7; } };
static int g_gathers = 0;
static CThing* g_owner = 0;
static NInventory::CCategory* g_category = 0;
struct CTCInventoryBase {
    long id;
    CThing* GetOwner() const { return g_owner; }
    void GetAllItems(CItemVector& items)
    {
        ++g_gathers;
        NInventory::CItem a = { id, id * 10 }; items.push_back(a);
        NInventory::CItem b = { id == 1 ? 7 : 0, 5 }; items.push_back(b);   // 7: not buyable, 0: no def
    }
    NInventory::CCategory* GetSelectedCategory() { return g_category; }
    long GetCategoryCount() const { return 9; }
};
static std::vector<long> g_added;
struct CTCInventoryTrade : CTCInventoryBase {
    long          CategoryCount;
    long          ItemSellCount;
    long          ItemBuyCount;
    long          ItemSellUniqueCount;
    long          CategoryIdent;
    bool AddItemOfTypeToInventory(long defIndex, long count) { g_added.push_back(defIndex); g_added.push_back(count); return true; }
    void InitialiseSellableItems();
};
void CTCInventoryTrade::InitialiseSellableItems()
{
    CategoryIdent = 0xc;
    long total = 0;
    CThing* hero = g_pEngine->PlayerManager->GetMainPlayer()->GetControlledCreature();
    CItemVector items;
    CTCInventoryBase* inventory = 0;
    hero->GetInterface(&inventory, 0x11);
    inventory->GetAllItems(items);
    hero->GetInterface(&inventory, 0x13);
    inventory->GetAllItems(items);
    hero->GetInterface(&inventory, 0x12);
    inventory->GetAllItems(items);
    bool isShop = GetOwner()->HasTC(0x53);
    for (unsigned long i = 0; i < items.size(); ++i) {
        long defIndex = items[i].DefIndex;
        if (defIndex > 0 && CTCInventoryItem::IsBuyable(defIndex)) {
            AddItemOfTypeToInventory(defIndex, items[i].Count);
            total += items[i].Count;
        }
    }
    ItemSellCount = total;
    NInventory::CCategory* category = GetSelectedCategory();
    if (category)
        ItemSellUniqueCount = category->GetUniqueItemCountInCategory();
    CategoryCount = GetCategoryCount();
}

int main()
{
    int fails = 0;
    CTCInventoryBase inv11 = { 1 }, inv13 = { 3 };
    CInterfaceMap::SEntry entries[2] = { { 0x11, (CTCBase*)&inv11 }, { 0x13, (CTCBase*)&inv13 } };
    CThing hero = {};
    hero.present.bits[0] = (1UL << 0x11) | (1UL << 0x13);       // no 0x12: the 0x13 inventory is gathered twice
    hero.m_interfaces.first = entries; hero.m_interfaces.last = entries + 2;
    g_hero = &hero;
    CThing owner = {}; g_owner = &owner;
    NInventory::CCategory cat = { 4 }; g_category = &cat;
    CTCInventoryTrade t; t.id = 0; t.CategoryCount = -1; t.ItemSellCount = -1; t.ItemSellUniqueCount = -1; t.CategoryIdent = -1;
    t.InitialiseSellableItems();
    if (t.CategoryIdent != 0xc || g_gathers != 3 || t.CategoryCount != 9 || t.ItemSellUniqueCount != 4) ++fails;
    // items: {1,10},{7,5},{3,30},{0,5},{3,30},{0,5} -> buyable: 1, 3, 3
    long expect[] = { 1, 10, 3, 30, 3, 30 };
    if (g_added.size() != 6) ++fails;
    else for (int i = 0; i < 6; ++i) if (g_added[i] != expect[i]) ++fails;
    if (t.ItemSellCount != 70) ++fails;
    g_category = 0; t.ItemSellUniqueCount = 77; g_added.clear();
    t.InitialiseSellableItems();
    if (t.ItemSellUniqueCount != 77 || t.ItemSellCount != 70) ++fails;
    if (fails) { std::printf("0059d820_TEST FAIL %d\n", fails); return 1; }
    std::printf("0059d820_TEST PASS\n");
    return 0;
}
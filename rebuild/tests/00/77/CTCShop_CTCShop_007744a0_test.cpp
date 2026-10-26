#include <cstdio>
// Behaviour test for CTCShop::CTCShop(CThing*) (0x007744A0): the body of src/ over stand-ins -- the defaults, every
// stock item of the shop def copied into the stock map with its quantity raised to at least the max stock, and the
// last-checked day set 30 days back.
template <class T> inline const T& Max(const T& a, const T& b) { return a < b ? b : a; }
class CShopItem { public:
    float Quantity;
    float PriceMultiplier;
    float MaxStock;
    long  RestockPeriod;
    float RestockPeriodRandomness;
    float SalesPerDay;
    float SalesPerDayRandomness;
};
class CStockItemDef { public: long Key; CShopItem Item; };
static int g_prepared = 0, g_releases = 0;
class CShopDef { public:
    long m_refCount;
    CStockItemDef* StockItemsFirst;
    CStockItemDef* StockItemsLast;
    void PrepareStockItems() { ++g_prepared; }
    void ReleaseReference() { --m_refCount; ++g_releases; }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->ReleaseReference(); }
    T* operator->() const { return Object; }
};
static CShopDef* g_shopDef = 0;
class CDefClassBase { public:
    long m_refCount;
    void ReleaseReference() { --m_refCount; ++g_releases; }
    bool GetSubDef(CDefPointer<CShopDef>* out) { if (!g_shopDef) return false; out->Object = g_shopDef; ++g_shopDef->m_refCount; return true; }
};
static CDefClassBase g_thingDef = { 1 };
static long g_defIndex = -1;
class CDefinitionManager { public:
    bool GetDef(long index, CDefPointer<CDefClassBase>* out) { g_defIndex = index; out->Object = &g_thingDef; ++g_thingDef.m_refCount; return true; }
};
static CDefinitionManager g_defs;
class CThing { public: unsigned short DefIndex; };
class CGameTimeManager { public: long m_day; };
static CGameTimeManager g_time = { 100 };
class CWorld { public: CGameTimeManager* m_pGameTimeManager; };
static CWorld g_world = { &g_time };
struct CStockMap {                                  // map<long, CShopItem> stand-in
    long keys[8]; CShopItem items[8]; int count;
    CStockMap() : count(0) {}
    CShopItem& operator[](const long& key)
    {
        for (int i = 0; i < count; ++i) if (keys[i] == key) return items[i];
        keys[count] = key; CShopItem zero = { 0, 0, 0, 0, 0, 0, 0 }; items[count] = zero; return items[count++];
    }
};
class CTCBase { public:
    CThing* ParentThing;
    CTCBase(CThing* thing) : ParentThing(thing) {}
    CDefinitionManager* GetDefinitions() const { return &g_defs; }
    CWorld* GetWorld() { return &g_world; }
};
class CTCShop : public CTCBase { public:
    long      StockState;
    CStockMap Stock;
    long      SpecialItem;
    long      DaysSinceSpecialItemPlaced;
    long      DayLastChecked;
    long      GoldDonated;
    bool      StallLike;
    unsigned long Alarm;
    CTCShop(CThing* thing);
};

CTCShop::CTCShop(CThing* thing)
    : CTCBase(thing), StockState(1), SpecialItem(-1), DaysSinceSpecialItemPlaced(0), GoldDonated(0), StallLike(false),
      Alarm(0)
{
    CDefPointer<CDefClassBase> thingDef;
    GetDefinitions()->GetDef(ParentThing->DefIndex, &thingDef);
    CDefPointer<CShopDef> shopDef;
    if (thingDef->GetSubDef(&shopDef)) {
        shopDef->PrepareStockItems();
        for (CStockItemDef* it = shopDef->StockItemsFirst; it != shopDef->StockItemsLast; ++it) {
            CShopItem& item = Stock[it->Key];
            item = it->Item;
            item.Quantity = Max(item.Quantity, item.MaxStock);
        }
    }
    DayLastChecked = GetWorld()->m_pGameTimeManager->m_day - 30;
}

static int fail(int line) { printf("007744a0_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) do { if (!(c)) return fail(__LINE__); } while (0)

int main()
{
    CThing thing = { 321 };
    {
        CTCShop plain(&thing);                          // no shop def: defaults only
        CHECK(plain.StockState == 1 && plain.SpecialItem == -1 && plain.DaysSinceSpecialItemPlaced == 0);
        CHECK(plain.GoldDonated == 0 && !plain.StallLike && plain.Alarm == 0 && plain.Stock.count == 0);
        CHECK(plain.DayLastChecked == 70 && g_defIndex == 321 && g_prepared == 0 && g_releases == 1);
    }
    CStockItemDef items[2] = {
        { 5, { 2.0f, 1.5f, 6.0f, 3, 0.1f, 2.0f, 0.2f } },   // quantity below max stock: raised to 6
        { 9, { 8.0f, 1.0f, 4.0f, 1, 0.0f, 1.0f, 0.0f } },   // quantity above max stock: kept
    };
    CShopDef shopDef = { 1, items, items + 2 };
    g_shopDef = &shopDef;
    CTCShop shop(&thing);
    CHECK(g_prepared == 1 && shop.Stock.count == 2);
    CHECK(shop.Stock.keys[0] == 5 && shop.Stock.items[0].Quantity == 6.0f && shop.Stock.items[0].PriceMultiplier == 1.5f);
    CHECK(shop.Stock.items[0].RestockPeriod == 3 && shop.Stock.items[0].SalesPerDayRandomness == 0.2f);
    CHECK(shop.Stock.keys[1] == 9 && shop.Stock.items[1].Quantity == 8.0f);
    CHECK(shopDef.m_refCount == 1 && g_thingDef.m_refCount == 1);
    printf("007744a0_TEST PASS\n");
    return 0;
}
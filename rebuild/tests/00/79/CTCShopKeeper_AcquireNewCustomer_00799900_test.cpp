#include <cstdio>
// Behaviour test for CTCShopKeeper::AcquireNewCustomer (0x00799900): the body of src/ over stand-ins -- the search box is
// the keeper's truncated position +-15, the nearest potential customer (to the shop) becomes the customer and is added;
// with none, the customer list is emptied only when not even an existing shopper is around.
static int g_added = 0, g_emptied = 0, g_best = 0, g_first = 0;
static long g_box[4];
static bool g_lastAllowExisting = false;
static float g_compareX = 0.0f;
class C3DVector { public: float X, Y, Z; };
class CThing { public: C3DVector pos; const C3DVector& GetPosition() const { return pos; } };
class CThingCreatureBase { public: int id; };
class C2DBoxI { public:
    long MinX, MinY, MaxX, MaxY;
    C2DBoxI(long minX, long minY, long maxX, long maxY) : MinX(minX), MinY(minY), MaxX(maxX), MaxY(maxY) { }
};
struct CBaseIntelligentPointer {
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    void* GetPItem() const { return m_pItem; }
    void SetPItem(void* item) { m_pItem = item; }
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {
    CIntelligentPointerCreature(CThingCreatureBase* const& item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerCreature(const CIntelligentPointerCreature& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    void operator=(const CIntelligentPointerCreature& other) { void* item = other.GetPItem(); SetPItem(item); }
};
class CTCShop { public: CThing* ParentThing; };
class CTCShopKeeper;
class CThingFilter_IsPotentialCustomer { public:
    CTCShopKeeper* TCShopKeeper;
    bool AllowExistingShopper;
    CThingFilter_IsPotentialCustomer(CTCShopKeeper* shopKeeper, bool allow) : TCShopKeeper(shopKeeper), AllowExistingShopper(allow) {}
};
class CThingCompare_Nearest { public: C3DVector Pos; CThingCompare_Nearest(const C3DVector& pos) : Pos(pos) { } };
static CThingCreatureBase* g_bestResult = 0;
static CThingCreatureBase* g_firstResult = 0;
class CThingSearchTools { public:
    bool GetPBestThingInRoughArea(const C2DBoxI& area, const CThingFilter_IsPotentialCustomer& filter,
                                  const CThingCompare_Nearest& compare, CThingCreatureBase** out)
    {
        ++g_best; g_box[0] = area.MinX; g_box[1] = area.MinY; g_box[2] = area.MaxX; g_box[3] = area.MaxY;
        g_lastAllowExisting = filter.AllowExistingShopper; g_compareX = compare.Pos.X;
        *out = g_bestResult; return g_bestResult != 0;
    }
    bool GetPFirstThingInRoughArea(const C2DBoxI&, const CThingFilter_IsPotentialCustomer& filter, CThingCreatureBase** out)
    { ++g_first; g_lastAllowExisting = filter.AllowExistingShopper; *out = g_firstResult; return g_firstResult != 0; }
};
static CThingSearchTools g_tools;
static CTCShop* g_shop = 0;
static void* g_addedItem = 0;
class CTCShopKeeper { public:
    CThing* ParentThing;
    CIntelligentPointerCreature Customer;
    CTCShopKeeper(CThing* t) : ParentThing(t), Customer(0) {}
    CThingSearchTools* GetThingSearchTools() const { return &g_tools; }
    CTCShop* GetShopTC() { return g_shop; }
    void AddNewCustomer(CIntelligentPointerCreature customer) { ++g_added; g_addedItem = customer.GetPItem(); }
    void EmptyCustomerList() { ++g_emptied; }
    void AcquireNewCustomer();
};

void CTCShopKeeper::AcquireNewCustomer()
{
    CTCShop* shop = GetShopTC();
    if (!shop)
        return;
    C3DVector shopPos = shop->ParentThing->GetPosition();
    const C3DVector& pos = ParentThing->GetPosition();
    long x = (long)pos.X;
    long y = (long)pos.Y;
    C2DBoxI area(x - 15, y - 15, x + 15, y + 15);
    CThingCreatureBase* customer = 0;
    GetThingSearchTools()->GetPBestThingInRoughArea(area, CThingFilter_IsPotentialCustomer(this, false),
        CThingCompare_Nearest(shopPos), &customer);
    Customer = CIntelligentPointerCreature(customer);
    if (Customer.GetPItem())
        AddNewCustomer(Customer);
    else if (!GetThingSearchTools()->GetPFirstThingInRoughArea(area, CThingFilter_IsPotentialCustomer(this, true), &customer))
        EmptyCustomerList();
}

static int fail(int line) { printf("00799900_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) do { if (!(c)) return fail(__LINE__); } while (0)

int main()
{
    CThing keeperThing = { { 100.7f, 50.2f, 0.0f } };
    CThing shopThing = { { 7.0f, 8.0f, 0.0f } };
    CTCShop shop = { &shopThing };
    CTCShopKeeper keeper(&keeperThing);
    keeper.AcquireNewCustomer();                          // no shop: nothing
    CHECK(g_best == 0);
    g_shop = &shop;
    CThingCreatureBase buyer = { 1 };
    g_bestResult = &buyer;
    keeper.AcquireNewCustomer();
    CHECK(g_best == 1 && g_box[0] == 85 && g_box[1] == 35 && g_box[2] == 115 && g_box[3] == 65 && g_compareX == 7.0f);
    CHECK(keeper.Customer.GetPItem() == &buyer && g_added == 1 && g_addedItem == &buyer && g_first == 0);
    g_bestResult = 0;                                     // nobody new; an existing shopper remains: keep the list
    CThingCreatureBase shopper = { 2 };
    g_firstResult = &shopper;
    keeper.AcquireNewCustomer();
    CHECK(keeper.Customer.GetPItem() == 0 && g_first == 1 && g_lastAllowExisting && g_emptied == 0 && g_added == 1);
    g_firstResult = 0;                                    // nobody at all: empty the list
    keeper.AcquireNewCustomer();
    CHECK(g_first == 2 && g_emptied == 1);
    printf("00799900_TEST PASS\n");
    return 0;
}
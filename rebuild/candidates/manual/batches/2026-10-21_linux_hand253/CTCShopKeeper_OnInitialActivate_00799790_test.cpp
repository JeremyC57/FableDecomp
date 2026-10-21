#include <cstdio>
// Behaviour test for CTCShopKeeper::OnInitialActivate (0x00799790): the body of src/ over stand-ins -- the keeper takes
// the shop its creature names; without one it adopts the first shop whose keeper is missing or dead; the shop is told
// about its keeper; the base activation always runs.
static int g_baseActivate = 0, g_searches = 0;
struct CThing;
struct CTCShop;
struct CBaseIntelligentPointer {
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    CThing* GetPItem() const { return (CThing*)m_pItem; }
    void SetPItem(void* item) { m_pItem = item; }
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(T* item) { SetPItem(item); }
    void operator=(const CIntelligentPointer& other) { CThing* item = other.GetPItem(); SetPItem(item); }
};
struct CThing {
    bool dead;
    CTCShop* shopTC;                                   // interface 0x53 when non-null
    CBaseIntelligentPointer ShopThing;
    unsigned char m_flags91;
    unsigned long HasTC(long type) const { return type == 0x53 && shopTC != 0; }
    CTCShop* GetTC(long) { return shopTC; }
};
struct CThingCreatureBase;
template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T storage[4];
    vector() : _M_start(storage), _M_finish(storage), _M_end_of_storage(storage + 4) {}
};
struct CThingFilter_HasTC { long m_type; CThingFilter_HasTC(long type) : m_type(type) { } };
static CThing* g_allShops[4]; static int g_numShops = 0;
struct CThingSearchTools {
    void GetAllThings(const CThingFilter_HasTC& filter, vector<CThing*>& out)
    { ++g_searches; for (int i = 0; i < g_numShops; ++i) if (g_allShops[i]->HasTC(filter.m_type)) *out._M_finish++ = g_allShops[i]; }
};
static CThingSearchTools g_tools;
struct CTCShop {
    CBaseIntelligentPointer PShopKeeper;
    CThing* boundKeeper;
    void SetShopKeeper(CThing* keeper) { boundKeeper = keeper; PShopKeeper.SetPItem(keeper); }
};
struct CTCShopKeeper {
    CThing* ParentThing;
    CIntelligentPointer<CThingCreatureBase> Customer;
    CIntelligentPointer<CThing> Shop;
    CTCShopKeeper(CThing* thing) : ParentThing(thing), Customer(0), Shop(0) {}
    CTCShop* GetShopTC() const { CThing* t = Shop.GetPItem(); return t ? t->shopTC : 0; }
    CThing* GetCreature() const { return ParentThing; }
    CThingSearchTools* GetThingSearchTools() const { return &g_tools; }
    void CTCBase_OnInitialActivate() { ++g_baseActivate; }
    void OnInitialActivate();
};

void CTCShopKeeper::OnInitialActivate()
{
    Shop = CIntelligentPointer<CThing>(GetCreature()->ShopThing.GetPItem());
    CTCShop* shop = GetShopTC();
    if (!shop) {
        vector<CThing*> shops;
        GetThingSearchTools()->GetAllThings(CThingFilter_HasTC(0x53), shops);
        for (CThing** it = shops._M_start; it != shops._M_finish; ++it) {
            CTCShop* candidate = 0;
            if ((*it)->HasTC(0x53))
                candidate = (CTCShop*)(*it)->GetTC(0x53);
            CThing* keeper = candidate->PShopKeeper.GetPItem();
            if (!keeper || (keeper->m_flags91 & 1)) {
                Shop = CIntelligentPointer<CThing>(*it);
                shop = candidate;
                break;
            }
        }
    }
    if (shop)
        shop->SetShopKeeper(ParentThing);
    CTCBase_OnInitialActivate();
}

static int fail(int line) { printf("00799790_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) do { if (!(c)) return fail(__LINE__); } while (0)

int main()
{
    CTCShop shops[3] = { { CBaseIntelligentPointer(), 0 }, { CBaseIntelligentPointer(), 0 }, { CBaseIntelligentPointer(), 0 } };
    CThing shopThings[3];
    CThing liveKeeper; liveKeeper.m_flags91 = 0; liveKeeper.shopTC = 0;
    CThing deadKeeper; deadKeeper.m_flags91 = 1; deadKeeper.shopTC = 0;
    for (int i = 0; i < 3; ++i) { shopThings[i].shopTC = &shops[i]; shopThings[i].m_flags91 = 0; g_allShops[i] = &shopThings[i]; }
    g_numShops = 3;
    shops[0].PShopKeeper.SetPItem(&liveKeeper);         // taken
    shops[1].PShopKeeper.SetPItem(&deadKeeper);         // keeper dead: free

    CThing creatureA; creatureA.shopTC = 0; creatureA.m_flags91 = 0;
    creatureA.ShopThing.SetPItem(&shopThings[2]);       // names its own shop
    CTCShopKeeper keeperA(&creatureA);
    keeperA.OnInitialActivate();
    CHECK(keeperA.Shop.GetPItem() == &shopThings[2] && shops[2].boundKeeper == &creatureA && g_searches == 0);
    CHECK(g_baseActivate == 1);

    CThing creatureB; creatureB.shopTC = 0; creatureB.m_flags91 = 0;   // no shop named: adopt the first free one
    CTCShopKeeper keeperB(&creatureB);
    keeperB.OnInitialActivate();
    CHECK(g_searches == 1 && keeperB.Shop.GetPItem() == &shopThings[1] && shops[1].boundKeeper == &creatureB);
    CHECK(g_baseActivate == 2);

    g_numShops = 1;                                     // only taken shops: nothing adopted
    CThing creatureC; creatureC.shopTC = 0; creatureC.m_flags91 = 0;
    CTCShopKeeper keeperC(&creatureC);
    keeperC.OnInitialActivate();
    CHECK(g_searches == 2 && keeperC.Shop.GetPItem() == 0 && shops[0].boundKeeper == 0 && g_baseActivate == 3);
    printf("00799790_TEST PASS\n");
    return 0;
}

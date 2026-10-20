// Behaviour test for 004e06c3: body copied from src/004e06c3.cpp, callees stubbed.
#include <cstdio>
#include <cstring>
#include <new>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
template <class P> struct CAllocProxy { P p; CAllocProxy(P v) : p(v) { } operator P() const { return p; } CAllocProxy& operator=(P v) { p = v; return *this; } };
static int g_baseCtor = 0, g_strCtor = 0, g_mapCtor = 0;
class CDefBase_0044c0c0 { public: CDefBase_0044c0c0() { ++g_baseCtor; } virtual ~CDefBase_0044c0c0() { } unsigned char _body[0x28 - 4]; };
class CCharString { public: void* m_pStringData; CCharString() : m_pStringData(0) { ++g_strCtor; } };
struct CShopItemVector { void* m_pFirst; void* m_pLast; CAllocProxy<void*> m_pEnd; CShopItemVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) {} };
class CAttitudeMultiplierMap { public: unsigned char _tree[12]; CAttitudeMultiplierMap() { ++g_mapCtor; _tree[0] = 7; } };
class CShopDef : public CDefBase_0044c0c0 {
public:
    unsigned long Name; unsigned long InitialHawkingPhrase; unsigned long ReplyToRequestPhrase; unsigned long CustomerFollowPhrase; unsigned long ThankingPhrase;
    CCharString ShopkeeperMeshType; CShopItemVector DefaultStock; CShopItemVector StockCategoryDefaults; float BuyPriceMultiplier; float SellPriceMultiplier;
    float AttitudeBuyPriceMultiplierDefault; CAttitudeMultiplierMap AttitudeBuyPriceMultiplier; float AttitudeSellPriceMultiplierDefault; CAttitudeMultiplierMap AttitudeSellPriceMultiplier;
    unsigned char _rest[0xd0 - 0x80];
    CShopDef();
    virtual ~CShopDef() { }
};
CShopDef::CShopDef()
{
}
int main()
{
    unsigned char raw[sizeof(CShopDef)]; memset(raw, 0xAA, sizeof(raw));
    CShopDef* def = new (raw) CShopDef();
    CHECK(g_baseCtor == 1 && g_strCtor == 1 && g_mapCtor == 2);
    CHECK(def->DefaultStock.m_pFirst == 0 && def->DefaultStock.m_pLast == 0 && def->DefaultStock.m_pEnd.p == 0);
    CHECK(def->StockCategoryDefaults.m_pFirst == 0 && def->StockCategoryDefaults.m_pLast == 0 && def->StockCategoryDefaults.m_pEnd.p == 0);
    CHECK(def->ShopkeeperMeshType.m_pStringData == 0 && def->AttitudeBuyPriceMultiplier._tree[0] == 7 && def->AttitudeSellPriceMultiplier._tree[0] == 7);
    CHECK(def->Name == 0xAAAAAAAA && *(unsigned long*)&def->BuyPriceMultiplier == 0xAAAAAAAA);
    def->~CShopDef();
    if (fails == 0) printf("004e06c3_TEST PASS\n"); else printf("004e06c3_TEST FAIL\n");
    return fails;
}
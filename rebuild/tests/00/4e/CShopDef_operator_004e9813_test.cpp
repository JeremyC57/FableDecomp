// Behaviour test for CShopDef::operator= (0x004E9813): the reconstructed body with recording stub callees.
// CShopDef::operator=(const CShopDef& other) -- memberwise def assignment: CDefClassBase part, the phrase ids, shopkeeper mesh name, the two stock vectors, price multipliers and attitude maps, stock/steal/donation tuning and special-item settings; returns *this
// Leaf: ??4CShopDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E9813, length 411 bytes; size-optimised def TU (odd address).
// Identity: the manifest label "CThingBuildingDef::operator=" is propagated over every def assignment; the body is reached
// through the `jmp` thunk 0x004E980E in slot 19 of vtable 0x01241F9C = CShopDef (rtti-vtables.tsv).
// Callees: CDefClassBase::operator= 0x00431F10, CCharString::operator= 0x0099EFB0, vector<CShopItemDef>::operator=
// 0x004E9AD3 (x2), map<EOpinionAttitudeType,float>::operator= 0x004E99AE (x2).
// Layout: Ego_r.pdb CShopDef (0xE0) with 16-byte vectors/maps; retail STLport containers are 12 bytes: DefaultStock +0x40,
// StockCategoryDefaults +0x4C, BuyPriceMultiplier +0x58, AttitudeBuyPriceMultiplier +0x64, AttitudeSellPriceMultiplierDefault
// +0x70, AttitudeSellPriceMultiplier +0x74, MaxStockChangePerSell +0x80 .. SpecialItemPriceMult +0xCC (retail sizeof 0xD0).
// Shape: the compiler-generated memberwise assignment written out in declaration order (CHeroExperienceDef 0x004F914A model);
// float members copy through eax.

class CDefClassBase { public:
    unsigned char _body[0x28];
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};
class CCharString { public:
    void* m_pStringData;
    const CCharString& operator=(const CCharString& other);       // 0x0099EFB0
};
struct CShopItemDefVector {                                 // vector<CShopItemDef> (12 bytes)
    void* m_first; void* m_last; void* m_end;
    CShopItemDefVector& operator=(const CShopItemDefVector& other);   // 0x004E9AD3
};
struct CAttitudeFloatMap {                                  // map<EOpinionAttitudeType,float> (12 bytes)
    void* head; unsigned long size; unsigned long _compare;
    CAttitudeFloatMap& operator=(const CAttitudeFloatMap& other);     // 0x004E99AE
};

class CShopDef : public CDefClassBase { public:                           // vtable 0x01241F9C
    unsigned long Name;                                     // +0x28
    unsigned long InitialHawkingPhrase;                     // +0x2c
    unsigned long ReplyToRequestPhrase;                     // +0x30
    unsigned long CustomerFollowPhrase;                     // +0x34
    unsigned long ThankingPhrase;                           // +0x38
    CCharString ShopkeeperMeshType;                         // +0x3c
    CShopItemDefVector DefaultStock;                        // +0x40
    CShopItemDefVector StockCategoryDefaults;               // +0x4c
    float BuyPriceMultiplier;                               // +0x58
    float SellPriceMultiplier;                              // +0x5c
    float AttitudeBuyPriceMultiplierDefault;                // +0x60
    CAttitudeFloatMap AttitudeBuyPriceMultiplier;           // +0x64
    float AttitudeSellPriceMultiplierDefault;               // +0x70
    CAttitudeFloatMap AttitudeSellPriceMultiplier;          // +0x74
    float MaxStockChangePerSell;                            // +0x80
    float MaxStockChangePerBuy;                             // +0x84
    float MaxStockRevertPerDay;                             // +0x88
    float PriceReductionFractionPerItem;                    // +0x8c
    float MaxPriceReductionFractionDueToStockCount;         // +0x90
    unsigned long PreferredShopkeeper;                      // +0x94
    bool IsStockDisplayPermanent;                           // +0x98
    bool IsBarberShop;                                      // +0x99
    bool IsTattooShop;                                      // +0x9a
    bool IsTitleShop;                                       // +0x9b
    bool AlwaysOpen;                                        // +0x9c
    float MinPriceMultiplierForItemToBeWanted;              // +0xa0
    float MaxPriceToDisplay;                                // +0xa4
    float MinTimeToSteal;                                   // +0xa8
    float MaxTimeToSteal;                                   // +0xac
    long MinGoldForDonation;                                // +0xb0
    long MaxGoldForDonation;                                // +0xb4
    float MaxMoralityForDonation;                           // +0xb8
    float LogarithmicPower;                                 // +0xbc
    long NumDaysToKeepSpecialItem;                          // +0xc0
    long MinAmountBeforeSpecialItem;                        // +0xc4
    float ProbabilityOfSpecialItemPerDay;                   // +0xc8
    float SpecialItemPriceMult;                             // +0xcc
    CShopDef& operator=(const CShopDef& other);
};

CShopDef& CShopDef::operator=(const CShopDef& other)
{
    CDefClassBase::operator=(other);
    Name = other.Name;
    InitialHawkingPhrase = other.InitialHawkingPhrase;
    ReplyToRequestPhrase = other.ReplyToRequestPhrase;
    CustomerFollowPhrase = other.CustomerFollowPhrase;
    ThankingPhrase = other.ThankingPhrase;
    ShopkeeperMeshType = other.ShopkeeperMeshType;
    DefaultStock = other.DefaultStock;
    StockCategoryDefaults = other.StockCategoryDefaults;
    BuyPriceMultiplier = other.BuyPriceMultiplier;
    SellPriceMultiplier = other.SellPriceMultiplier;
    AttitudeBuyPriceMultiplierDefault = other.AttitudeBuyPriceMultiplierDefault;
    AttitudeBuyPriceMultiplier = other.AttitudeBuyPriceMultiplier;
    AttitudeSellPriceMultiplierDefault = other.AttitudeSellPriceMultiplierDefault;
    AttitudeSellPriceMultiplier = other.AttitudeSellPriceMultiplier;
    MaxStockChangePerSell = other.MaxStockChangePerSell;
    MaxStockChangePerBuy = other.MaxStockChangePerBuy;
    MaxStockRevertPerDay = other.MaxStockRevertPerDay;
    PriceReductionFractionPerItem = other.PriceReductionFractionPerItem;
    MaxPriceReductionFractionDueToStockCount = other.MaxPriceReductionFractionDueToStockCount;
    PreferredShopkeeper = other.PreferredShopkeeper;
    IsStockDisplayPermanent = other.IsStockDisplayPermanent;
    IsBarberShop = other.IsBarberShop;
    IsTattooShop = other.IsTattooShop;
    IsTitleShop = other.IsTitleShop;
    AlwaysOpen = other.AlwaysOpen;
    MinPriceMultiplierForItemToBeWanted = other.MinPriceMultiplierForItemToBeWanted;
    MaxPriceToDisplay = other.MaxPriceToDisplay;
    MinTimeToSteal = other.MinTimeToSteal;
    MaxTimeToSteal = other.MaxTimeToSteal;
    MinGoldForDonation = other.MinGoldForDonation;
    MaxGoldForDonation = other.MaxGoldForDonation;
    MaxMoralityForDonation = other.MaxMoralityForDonation;
    LogarithmicPower = other.LogarithmicPower;
    NumDaysToKeepSpecialItem = other.NumDaysToKeepSpecialItem;
    MinAmountBeforeSpecialItem = other.MinAmountBeforeSpecialItem;
    ProbabilityOfSpecialItemPerDay = other.ProbabilityOfSpecialItemPerDay;
    SpecialItemPriceMult = other.SpecialItemPriceMult;
    return *this;
}

static int g_base = 0, g_str = 0, g_vec = 0, g_map = 0;
CDefClassBase& CDefClassBase::operator=(const CDefClassBase& o) { for (int i = 0; i < 0x28; ++i) _body[i] = o._body[i]; ++g_base; return *this; }
const CCharString& CCharString::operator=(const CCharString& o) { m_pStringData = o.m_pStringData; ++g_str; return *this; }
CShopItemDefVector& CShopItemDefVector::operator=(const CShopItemDefVector& o) { m_last = o.m_last; ++g_vec; return *this; }
CAttitudeFloatMap& CAttitudeFloatMap::operator=(const CAttitudeFloatMap& o) { size = o.size; ++g_map; return *this; }
#include <cstdio>
#include <cstring>
static CShopDef a, b;
int main()
{
    std::memset(&a, 0, sizeof(a));
    std::memset(&b, 0, sizeof(b));
    a._body[1] = 3; a.Name = 1; a.ThankingPhrase = 5; a.ShopkeeperMeshType.m_pStringData = &a; a.DefaultStock.m_last = &b;
    a.StockCategoryDefaults.m_last = &a; a.BuyPriceMultiplier = 1.25f; a.SellPriceMultiplier = 0.5f;
    a.AttitudeBuyPriceMultiplier.size = 4; a.AttitudeSellPriceMultiplierDefault = 2.0f; a.AttitudeSellPriceMultiplier.size = 6;
    a.MaxPriceReductionFractionDueToStockCount = 0.125f; a.PreferredShopkeeper = 77; a.IsBarberShop = true; a.AlwaysOpen = true;
    a.MinGoldForDonation = 10; a.MaxGoldForDonation = 100; a.NumDaysToKeepSpecialItem = 3; a.SpecialItemPriceMult = 4.5f;
    CShopDef* r = &(b = a);
    bool ok = r == &b && g_base == 1 && g_str == 1 && g_vec == 2 && g_map == 2 && b._body[1] == 3 && b.Name == 1
        && b.ThankingPhrase == 5 && b.ShopkeeperMeshType.m_pStringData == &a && b.DefaultStock.m_last == &b
        && b.StockCategoryDefaults.m_last == &a && b.BuyPriceMultiplier == 1.25f && b.SellPriceMultiplier == 0.5f
        && b.AttitudeBuyPriceMultiplier.size == 4 && b.AttitudeSellPriceMultiplierDefault == 2.0f
        && b.AttitudeSellPriceMultiplier.size == 6 && b.MaxPriceReductionFractionDueToStockCount == 0.125f
        && b.PreferredShopkeeper == 77 && !b.IsStockDisplayPermanent && b.IsBarberShop && b.AlwaysOpen
        && b.MinGoldForDonation == 10 && b.MaxGoldForDonation == 100 && b.NumDaysToKeepSpecialItem == 3
        && b.SpecialItemPriceMult == 4.5f;
    if (!ok) { std::printf("004e9813_TEST FAIL\n"); return 1; }
    std::printf("004e9813_TEST PASS\n");
    return 0;
}
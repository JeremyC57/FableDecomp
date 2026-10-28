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
#pragma optimize("s", on)

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
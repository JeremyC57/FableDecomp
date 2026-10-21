// CShopDef::Transfer(CPersistContext&) -- serialise every CShopDef field through the persist context
// Retail 0x004ECB4D, 489 bytes, `ret 4`. One CPersistContext::Transfer overload per member, in declared
// order except PreferredShopkeeper (+0x94) comes after IsStockDisplayPermanent, the tattoo flag before the
// barber flag, and MinPriceMultiplierForItemToBeWanted before AlwaysOpen.
// Callees: Transfer(unsigned long&) 0x00431020, Transfer(CCharString&) 0x00431143,
// Transfer(vector<CShopItemDef>&) 0x004ECF6E, Transfer(float&) 0x00431061,
// Transfer(map<EOpinionAttitudeType,float>&) 0x004ECD36, Transfer(bool&) 0x0043314A,
// Transfer(long&) 0x00431102.
// Layout: Ego_r.pdb CShopDef (0xE0) uses 16-byte vectors/maps; retail STLport vectors and maps are 12 bytes,
// so DefaultStock..AttitudeSellPriceMultiplier sit 4..16 bytes below their PDB offsets (retail size 0xD0).
// The containers are opaque three-word views here; no engine header exists.
// Size-optimised TU: `#pragma optimize("s", on)` (every address goes through eax, as in retail).
#pragma optimize("s", on)
struct CCharString { void* Data; };
struct CShopItemDefVector { void* _M_start; void* _M_finish; void* _M_end_of_storage; };
struct COpinionAttitudeFloatMap { void* _M_header; unsigned long _M_node_count; unsigned long _M_compare; };

struct CPersistContext {
    void Transfer(unsigned long& value);            // 0x00431020
    void Transfer(CCharString& value);              // 0x00431143
    void Transfer(CShopItemDefVector& value);       // 0x004ECF6E
    void Transfer(float& value);                    // 0x00431061
    void Transfer(COpinionAttitudeFloatMap& value); // 0x004ECD36
    void Transfer(bool& value);                     // 0x0043314A
    void Transfer(long& value);                     // 0x00431102
};

struct CShopDef {
    unsigned char _defBase[0x24];                   // +0x04 after the vptr
    unsigned long Name;                             // +0x28
    unsigned long InitialHawkingPhrase;             // +0x2C
    unsigned long ReplyToRequestPhrase;             // +0x30
    unsigned long CustomerFollowPhrase;             // +0x34
    unsigned long ThankingPhrase;                   // +0x38
    CCharString ShopkeeperMeshType;                 // +0x3C
    CShopItemDefVector DefaultStock;                // +0x40
    CShopItemDefVector StockCategoryDefaults;       // +0x4C
    float BuyPriceMultiplier;                       // +0x58
    float SellPriceMultiplier;                      // +0x5C
    float AttitudeBuyPriceMultiplierDefault;        // +0x60
    COpinionAttitudeFloatMap AttitudeBuyPriceMultiplier;  // +0x64
    float AttitudeSellPriceMultiplierDefault;       // +0x70
    COpinionAttitudeFloatMap AttitudeSellPriceMultiplier; // +0x74
    float MaxStockChangePerSell;                    // +0x80
    float MaxStockChangePerBuy;                     // +0x84
    float MaxStockRevertPerDay;                     // +0x88
    float PriceReductionFractionPerItem;            // +0x8C
    float MaxPriceReductionFractionDueToStockCount; // +0x90
    unsigned long PreferredShopkeeper;              // +0x94
    bool IsStockDisplayPermanent;                   // +0x98
    bool IsBarberShop;                              // +0x99
    bool IsTattooShop;                              // +0x9A
    bool IsTitleShop;                               // +0x9B
    bool AlwaysOpen;                                // +0x9C
    float MinPriceMultiplierForItemToBeWanted;      // +0xA0
    float MaxPriceToDisplay;                        // +0xA4
    float MinTimeToSteal;                           // +0xA8
    float MaxTimeToSteal;                           // +0xAC
    long MinGoldForDonation;                        // +0xB0
    long MaxGoldForDonation;                        // +0xB4
    float MaxMoralityForDonation;                   // +0xB8
    float LogarithmicPower;                         // +0xBC
    long NumDaysToKeepSpecialItem;                  // +0xC0
    long MinAmountBeforeSpecialItem;                // +0xC4
    float ProbabilityOfSpecialItemPerDay;           // +0xC8
    float SpecialItemPriceMult;                     // +0xCC
    virtual void Transfer(CPersistContext& context);
};

void CShopDef::Transfer(CPersistContext& context)
{
    context.Transfer(Name);
    context.Transfer(InitialHawkingPhrase);
    context.Transfer(ReplyToRequestPhrase);
    context.Transfer(CustomerFollowPhrase);
    context.Transfer(ThankingPhrase);
    context.Transfer(ShopkeeperMeshType);
    context.Transfer(DefaultStock);
    context.Transfer(StockCategoryDefaults);
    context.Transfer(BuyPriceMultiplier);
    context.Transfer(SellPriceMultiplier);
    context.Transfer(AttitudeBuyPriceMultiplierDefault);
    context.Transfer(AttitudeBuyPriceMultiplier);
    context.Transfer(AttitudeSellPriceMultiplierDefault);
    context.Transfer(AttitudeSellPriceMultiplier);
    context.Transfer(MaxStockChangePerSell);
    context.Transfer(MaxStockChangePerBuy);
    context.Transfer(MaxStockRevertPerDay);
    context.Transfer(PriceReductionFractionPerItem);
    context.Transfer(MaxPriceReductionFractionDueToStockCount);
    context.Transfer(IsStockDisplayPermanent);
    context.Transfer(PreferredShopkeeper);
    context.Transfer(IsTattooShop);
    context.Transfer(IsBarberShop);
    context.Transfer(IsTitleShop);
    context.Transfer(MinPriceMultiplierForItemToBeWanted);
    context.Transfer(AlwaysOpen);
    context.Transfer(MaxPriceToDisplay);
    context.Transfer(MinTimeToSteal);
    context.Transfer(MaxTimeToSteal);
    context.Transfer(MinGoldForDonation);
    context.Transfer(MaxGoldForDonation);
    context.Transfer(MaxMoralityForDonation);
    context.Transfer(LogarithmicPower);
    context.Transfer(NumDaysToKeepSpecialItem);
    context.Transfer(MinAmountBeforeSpecialItem);
    context.Transfer(ProbabilityOfSpecialItemPerDay);
    context.Transfer(SpecialItemPriceMult);
}

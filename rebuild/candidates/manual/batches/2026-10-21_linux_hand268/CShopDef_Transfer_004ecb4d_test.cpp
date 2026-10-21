// Behaviour test for CShopDef::Transfer (0x004ECB4D): every member reaches the persist context once, in
// retail's order, through the overload of its type.
#include <cstdio>
struct CCharString { void* Data; };
struct CShopItemDefVector { void* a; void* b; void* c; };
struct COpinionAttitudeFloatMap { void* a; unsigned long b; unsigned long c; };
static const void* g_seen[64];
static char g_kind[64];
static int g_count = 0;
static void Note(const void* p, char k) { if (g_count < 64) { g_seen[g_count] = p; g_kind[g_count] = k; } ++g_count; }
struct CPersistContext {
    void Transfer(unsigned long& v) { Note(&v, 'u'); }
    void Transfer(CCharString& v) { Note(&v, 's'); }
    void Transfer(CShopItemDefVector& v) { Note(&v, 'v'); }
    void Transfer(float& v) { Note(&v, 'f'); }
    void Transfer(COpinionAttitudeFloatMap& v) { Note(&v, 'm'); }
    void Transfer(bool& v) { Note(&v, 'b'); }
    void Transfer(long& v) { Note(&v, 'l'); }
};
struct CShopDef {
    unsigned long Name, InitialHawkingPhrase, ReplyToRequestPhrase, CustomerFollowPhrase, ThankingPhrase;
    CCharString ShopkeeperMeshType; CShopItemDefVector DefaultStock; CShopItemDefVector StockCategoryDefaults;
    float BuyPriceMultiplier, SellPriceMultiplier, AttitudeBuyPriceMultiplierDefault;
    COpinionAttitudeFloatMap AttitudeBuyPriceMultiplier; float AttitudeSellPriceMultiplierDefault;
    COpinionAttitudeFloatMap AttitudeSellPriceMultiplier;
    float MaxStockChangePerSell, MaxStockChangePerBuy, MaxStockRevertPerDay, PriceReductionFractionPerItem,
        MaxPriceReductionFractionDueToStockCount;
    unsigned long PreferredShopkeeper;
    bool IsStockDisplayPermanent, IsBarberShop, IsTattooShop, IsTitleShop, AlwaysOpen;
    float MinPriceMultiplierForItemToBeWanted, MaxPriceToDisplay, MinTimeToSteal, MaxTimeToSteal;
    long MinGoldForDonation, MaxGoldForDonation; float MaxMoralityForDonation, LogarithmicPower;
    long NumDaysToKeepSpecialItem, MinAmountBeforeSpecialItem; float ProbabilityOfSpecialItemPerDay, SpecialItemPriceMult;
    void Transfer(CPersistContext& context);
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
int main()
{
    CShopDef d; CPersistContext c; d.Transfer(c);
    const void* want[37] = { &d.Name, &d.InitialHawkingPhrase, &d.ReplyToRequestPhrase, &d.CustomerFollowPhrase,
        &d.ThankingPhrase, &d.ShopkeeperMeshType, &d.DefaultStock, &d.StockCategoryDefaults, &d.BuyPriceMultiplier,
        &d.SellPriceMultiplier, &d.AttitudeBuyPriceMultiplierDefault, &d.AttitudeBuyPriceMultiplier,
        &d.AttitudeSellPriceMultiplierDefault, &d.AttitudeSellPriceMultiplier, &d.MaxStockChangePerSell,
        &d.MaxStockChangePerBuy, &d.MaxStockRevertPerDay, &d.PriceReductionFractionPerItem,
        &d.MaxPriceReductionFractionDueToStockCount, &d.IsStockDisplayPermanent, &d.PreferredShopkeeper,
        &d.IsTattooShop, &d.IsBarberShop, &d.IsTitleShop, &d.MinPriceMultiplierForItemToBeWanted, &d.AlwaysOpen,
        &d.MaxPriceToDisplay, &d.MinTimeToSteal, &d.MaxTimeToSteal, &d.MinGoldForDonation, &d.MaxGoldForDonation,
        &d.MaxMoralityForDonation, &d.LogarithmicPower, &d.NumDaysToKeepSpecialItem, &d.MinAmountBeforeSpecialItem,
        &d.ProbabilityOfSpecialItemPerDay, &d.SpecialItemPriceMult };
    const char* kinds = "uuuuusvvfffmfmfffffbubbbfbfffllffllff";
    if (g_count != 37) { std::printf("004ecb4d_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 37; ++i)
        if (g_seen[i] != want[i] || g_kind[i] != kinds[i]) { std::printf("004ecb4d_TEST FAIL\n"); return 1; }
    std::printf("004ecb4d_TEST PASS\n");
    return 0;
}

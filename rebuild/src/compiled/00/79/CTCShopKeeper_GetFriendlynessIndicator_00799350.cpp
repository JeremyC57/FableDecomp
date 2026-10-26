// CTCShopKeeper::GetFriendlynessIndicator() const -- text key for the keeper's attitude: compare the shop's base price multiplier against its attitude multiplier and return LIKES (< 0.2), NEUTRAL (< 0.55) or DISLIKES; "" without a shop
// Leaf: GetFriendlynessIndicator
// Retail: Fable.exe 0x00799350, 151 bytes (`ret 4`, CCharString returned through the hidden pointer).
// Callees: CTCShopKeeper::GetShopTC() const @ 0x00798F50, two CTCShop float getters @ 0x00773790 and @ 0x007735D0
// (both labelled "GetAttitudeBuyPriceMultiplier" in the manifest; one is a propagation, names unverified),
// CCharString::CCharString(const char*, long length = -1) @ 0x0099EBF0.
// Globals: 0.2f @ 0x012305A0, 0.55f @ 0x0126663C; literals "TEXT_AI_SHOP_KEEPER_SHOW_WARES_LIKES" @ 0x0126C214,
// "..._NEUTRAL" @ 0x0126C1EC, "..._DISLIKES" @ 0x0126C1C4, "" @ 0x0122D70E.
// Shape: difference = base - attitude (`fsubr`), kept on the FPU stack across both compares; the -1 length push is
// shared (hoisted) by the three string returns.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
};
extern const float g_shopLikesThreshold;            // 0x012305A0 (0.2f)
extern const float g_shopNeutralThreshold;          // 0x0126663C (0.55f)
class CTCShop { public:
    float GetBasePriceMultiplier() const;           // 0x00773790
    float GetAttitudeBuyPriceMultiplier() const;    // 0x007735D0
};
class CTCShopKeeper { public:
    CTCShop* GetShopTC() const;                     // 0x00798F50
    CCharString GetFriendlynessIndicator() const;
};

CCharString CTCShopKeeper::GetFriendlynessIndicator() const
{
    CTCShop* shop = GetShopTC();
    if (shop) {
        float difference = shop->GetBasePriceMultiplier() - shop->GetAttitudeBuyPriceMultiplier();
        if (difference < g_shopLikesThreshold)
            return CCharString("TEXT_AI_SHOP_KEEPER_SHOW_WARES_LIKES");
        if (difference < g_shopNeutralThreshold)
            return CCharString("TEXT_AI_SHOP_KEEPER_SHOW_WARES_NEUTRAL");
        return CCharString("TEXT_AI_SHOP_KEEPER_SHOW_WARES_DISLIKES");
    }
    return CCharString("");
}
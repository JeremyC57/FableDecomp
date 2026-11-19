// Behaviour test for CTCShop::GetCurrentBasicPrice (0x007727E0), a portable model: 0 when the shop thing's def has no CShopDef;
// otherwise the item's DefaultPrice (1 when the item def has no CStockItemDef), reduced by PriceReductionFractionPerItem per held
// item (Quantity rounded up, plus `add`) capped at MaxPriceReductionFractionDueToStockCount, then times the default stock item's
// PriceMultiplier when there is one.
#include <stdio.h>
#include <map>
struct ShopDef { float PriceReductionFractionPerItem, MaxPriceReductionFractionDueToStockCount; };
struct ShopItem { float Quantity, PriceMultiplier; };
struct Shop {
    const ShopDef* shop_def;                    // 0: the thing's def has no shop sub def
    std::map<long, long> default_prices;        // item def -> CStockItemDef::DefaultPrice (absent: no stock-item sub def)
    std::map<long, ShopItem> stock;
    std::map<long, float> default_stock;        // item def -> default stock item's PriceMultiplier
};
static long CeilLong(float v) { long r = (long)v; if ((float)r < v) ++r; return r; }   // GFFloatToLongCeil for these values
static float GetCurrentBasicPrice(const Shop& s, long def, long add)
{
    if (!s.shop_def) return 0.0f;
    std::map<long, long>::const_iterator dp = s.default_prices.find(def);
    float price = dp != s.default_prices.end() ? (float)dp->second : 1.0f;
    std::map<long, ShopItem>::const_iterator it = s.stock.find(def);
    if (it != s.stock.end()) {
        float reduction = s.shop_def->PriceReductionFractionPerItem * (CeilLong(it->second.Quantity) + add);
        if (s.shop_def->MaxPriceReductionFractionDueToStockCount < reduction) reduction = s.shop_def->MaxPriceReductionFractionDueToStockCount;
        price *= 1.0f - reduction;
    }
    std::map<long, float>::const_iterator ds = s.default_stock.find(def);
    if (ds != s.default_stock.end()) price *= ds->second;
    return price;
}
int main()
{
    int bad = 0;
    ShopDef sd = { 0.125f, 0.5f };
    Shop s; s.shop_def = &sd;
    s.default_prices[7] = 100;
    ShopItem held = { 2.5f, 1.0f }; s.stock[7] = held;
    if (GetCurrentBasicPrice(s, 7, 0) != 62.5f) ++bad;                // ceil(2.5) = 3 items: 1 - 0.375
    if (GetCurrentBasicPrice(s, 7, 2) != 50.0f) ++bad;                // 5 items: 0.625 capped at 0.5
    s.default_stock[7] = 2.0f;
    if (GetCurrentBasicPrice(s, 7, 0) != 125.0f) ++bad;               // default stock multiplier
    ShopItem whole = { 2.0f, 1.0f }; s.stock[8] = whole;
    if (GetCurrentBasicPrice(s, 8, 0) != 0.75f) ++bad;                // no stock-item def: base 1; ceil(2.0) = 2
    if (GetCurrentBasicPrice(s, 9, 0) != 1.0f) ++bad;                 // not stocked, no defaults
    Shop none; none.shop_def = 0;
    if (GetCurrentBasicPrice(none, 7, 0) != 0.0f) ++bad;              // the thing has no shop def
    printf(bad ? "007727e0_TEST FAIL\n" : "007727e0_TEST PASS\n");
    return bad ? 1 : 0;
}

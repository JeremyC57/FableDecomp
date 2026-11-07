// Behaviour test for CTCShop::UpdateStock (0x00775350): a copy of the stock-advance logic over local models (deterministic seed
// and rand); checks the per-day revert of MaxStock towards the def default, the restock on the restock period and the daily
// sales otherwise, the bulk revert after a gap of more than 100000 days, the pruning of unlisted empty never-restocking items,
// the special item's expiry and its placement from the donated gold, and DayLastChecked.
#include <cstdio>
#include <cstring>
#include <math.h>
#include <map>
#include <vector>
#include <algorithm>

struct CShopItem { float Quantity, PriceMultiplier, MaxStock; long RestockPeriod, RestockPeriodRandomness; float SalesPerDay, SalesPerDayRandomness; };
struct CShopItemDef { long Type; CShopItem Item; };
struct CShopDef { std::vector<CShopItemDef> DefaultStock; float MaxStockRevertPerDay; long NumDaysToKeepSpecialItem, MinAmountBeforeSpecialItem; float ProbabilityOfSpecialItemPerDay, SpecialItemPriceMult; };
static float FMax(float a, float b) { return a < b ? b : a; }
static float MoveTowards(float value, float target, float step) { float diff = value - target; if (fabsf(diff) < step) return target; if (diff > 0.0f) return value - step; return value + step; }
static unsigned long g_seed = 1; static int g_rand = 0;
static float GFFloatRandom(float, unsigned long* seed) { *seed = *seed * 0x24a1 + 0x24df; return 0.0f; }
static float GFUnitRandom(unsigned long* seed) { *seed = *seed * 0x24a1 + 0x24df; return (float)((*seed >> 13) & 0xffff) / 65536.0f; }
static int fake_rand() { return g_rand++; }
static CShopItem Item(float q, float pm, float ms, long period, float sales) { CShopItem i; i.Quantity = q; i.PriceMultiplier = pm; i.MaxStock = ms; i.RestockPeriod = period; i.RestockPeriodRandomness = 0; i.SalesPerDay = sales; i.SalesPerDayRandomness = 0; return i; }
struct CTCShop {
    const CShopDef* shopDef; long day;
    std::map<long, CShopItem> Stock; long SpecialItem, DaysSinceSpecialItemPlaced, DayLastChecked, GoldDonated;
    // the landed 0x00772250 falls back to the def's category stock entry; the row dereferences the result without a null test
    const CShopItem* FindDefaultStockItem(long type) const { static CShopItem category = Item(0.0f, 1.0f, 0.0f, 0, 0.0f); for (size_t i = 0; i < shopDef->DefaultStock.size(); ++i) if (shopDef->DefaultStock[i].Type == type) return &shopDef->DefaultStock[i].Item; return &category; }
    void UpdateStock()
    {
        long daysSince = day - DayLastChecked;
        for (std::map<long, CShopItem>::iterator it = Stock.begin(); it != Stock.end(); ++it) {
            CShopItem& item = it->second;
            if (daysSince > 100000) {
                long period = item.RestockPeriod;
                if (period == 0) item.MaxStock = MoveTowards(item.MaxStock, FindDefaultStockItem(it->first)->MaxStock, (float)daysSince * shopDef->MaxStockRevertPerDay);
                else {
                    long lastRestock = ((day - 100000) / period) * period;
                    if (lastRestock > DayLastChecked) {
                        float days = (float)daysSince;
                        item.MaxStock = MoveTowards(item.MaxStock, FindDefaultStockItem(it->first)->MaxStock, (float)(lastRestock - DayLastChecked) * shopDef->MaxStockRevertPerDay * days);
                        item.MaxStock = MoveTowards(item.MaxStock, FindDefaultStockItem(it->first)->MaxStock, (float)(day - lastRestock - 100000) * shopDef->MaxStockRevertPerDay * days);
                    } else item.MaxStock = MoveTowards(item.MaxStock, FindDefaultStockItem(it->first)->MaxStock, (float)daysSince * shopDef->MaxStockRevertPerDay);
                }
            } else {
                for (long d = DayLastChecked + 1; d <= day; d++) {
                    item.MaxStock = MoveTowards(item.MaxStock, FindDefaultStockItem(it->first)->MaxStock, shopDef->MaxStockRevertPerDay);
                    if (item.RestockPeriod != 0) {
                        if (d % item.RestockPeriod == 0) item.Quantity = FMax(item.Quantity, item.MaxStock);
                        else { GFFloatRandom(1.0f, &g_seed); item.Quantity = FMax(item.Quantity - item.SalesPerDay, 0.0f); }
                    }
                }
            }
        }
        std::vector<long> listed;
        for (size_t i = 0; i < shopDef->DefaultStock.size(); ++i) listed.push_back(shopDef->DefaultStock[i].Type);
        std::sort(listed.begin(), listed.end());
        std::vector<long> unlisted;
        for (std::map<long, CShopItem>::iterator it = Stock.begin(); it != Stock.end(); ++it)
            if (!std::binary_search(listed.begin(), listed.end(), it->first) && it->second.Quantity == 0.0f && (it->second.RestockPeriod == 0 || it->second.MaxStock == 0.0f)) unlisted.push_back(it->first);
        for (size_t i = 0; i < unlisted.size(); ++i) Stock.erase(unlisted[i]);
        if (SpecialItem != -1) {
            DaysSinceSpecialItemPlaced += daysSince;
            if (DaysSinceSpecialItemPlaced > shopDef->NumDaysToKeepSpecialItem) { Stock.erase(SpecialItem); SpecialItem = -1; }
        }
        if (SpecialItem == -1 && GoldDonated > shopDef->MinAmountBeforeSpecialItem) {
            float chance = (float)pow(1.0f - shopDef->ProbabilityOfSpecialItemPerDay, (float)daysSince);
            if (GFUnitRandom(&g_seed) >= chance) {
                GoldDonated -= shopDef->MinAmountBeforeSpecialItem;
                std::vector<long> candidates;
                for (size_t i = 0; i < shopDef->DefaultStock.size(); ++i)
                    if (fabsf(shopDef->DefaultStock[i].Item.PriceMultiplier - shopDef->SpecialItemPriceMult) < 0.0001f) candidates.push_back(shopDef->DefaultStock[i].Type);
                if (candidates.size() != 0) {
                    for (size_t i = 1; i < candidates.size(); ++i) std::swap(candidates[i], candidates[fake_rand() % (i + 1)]);
                    SpecialItem = candidates[0];
                    const CShopItem* def = FindDefaultStockItem(SpecialItem);
                    if (def) { Stock[SpecialItem] = *def; Stock[SpecialItem].Quantity = 1.0f; }
                    DaysSinceSpecialItemPlaced = 0;
                }
            }
        }
        DayLastChecked = day;
    }
};
static bool Near(float a, float b) { return fabsf(a - b) < 1e-4f; }
int main()
{
    CShopDef d; d.MaxStockRevertPerDay = 1.0f; d.NumDaysToKeepSpecialItem = 3; d.MinAmountBeforeSpecialItem = 100; d.ProbabilityOfSpecialItemPerDay = 1.0f; d.SpecialItemPriceMult = 2.0f;
    CShopItemDef a = { 10, Item(5.0f, 1.0f, 10.0f, 4, 1.0f) }, b = { 20, Item(1.0f, 2.0f, 1.0f, 0, 0.0f) }, c = { 30, Item(1.0f, 2.0f, 1.0f, 0, 0.0f) };
    d.DefaultStock.push_back(a); d.DefaultStock.push_back(b); d.DefaultStock.push_back(c);
    CTCShop s; s.shopDef = &d; s.DayLastChecked = 0; s.day = 4; s.SpecialItem = -1; s.DaysSinceSpecialItemPlaced = 0; s.GoldDonated = 0;
    s.Stock[10] = Item(5.0f, 1.0f, 13.0f, 4, 1.0f);     // listed: MaxStock 13 reverts 1/day to 10; sells 1/day for 3 days then restocks on day 4
    s.Stock[99] = Item(0.0f, 1.0f, 0.0f, 0, 0.0f);      // unlisted, empty, never restocks: pruned
    s.UpdateStock();
    if (s.Stock.size() != 1 || !Near(s.Stock[10].MaxStock, 10.0f) || !Near(s.Stock[10].Quantity, 10.0f) || s.DayLastChecked != 4 || s.SpecialItem != -1) { std::printf("00775350_TEST FAIL\n"); return 1; }
    // donated gold places a special item (probability 1 per day -> chance 0, the roll always passes); its quantity is 1
    s.GoldDonated = 150; s.day = 5; s.UpdateStock();
    if (s.SpecialItem == -1 || (s.SpecialItem != 20 && s.SpecialItem != 30) || s.GoldDonated != 50 || !Near(s.Stock[s.SpecialItem].Quantity, 1.0f) || s.DaysSinceSpecialItemPlaced != 0) { std::printf("00775350_TEST FAIL\n"); return 1; }
    // the special item expires after more than NumDaysToKeepSpecialItem days
    long special = s.SpecialItem; s.day = 9; s.UpdateStock();
    if (s.SpecialItem != -1 || s.Stock.find(special) != s.Stock.end()) { std::printf("00775350_TEST FAIL\n"); return 1; }
    // bulk revert after a long gap: MaxStock snaps to the default
    s.Stock[10].MaxStock = 50.0f; s.day = 9 + 200000; s.UpdateStock();
    if (!Near(s.Stock[10].MaxStock, 10.0f) || s.DayLastChecked != 200009) { std::printf("00775350_TEST FAIL\n"); return 1; }
    std::printf("00775350_TEST PASS\n");
    return 0;
}

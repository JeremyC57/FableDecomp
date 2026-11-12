// Behaviour test for CTCShop::GetSaleItemsToPlace 0x00773EE0: filter stock entries by key > 0, quantity > 0, price cap and
// display flag; empty result without a shop def; result is a permutation of the filtered keys.
#include <cstdio>
#include <map>
#include <vector>
#include <algorithm>
class CShopItem { public: float Quantity; float PriceMultiplier; };
class CStockItemDef { public: long DefaultPrice; bool CanBeDisplayedInShop; };
class CShopDef { public: float MaxPriceToDisplay; };
struct Model {
    const CShopDef* shopDef;
    std::map<long, CShopItem> Stock;
    std::map<long, CStockItemDef> defs;
    std::vector<long> GetSaleItemsToPlace() const
    {
        std::vector<long> items;
        bool noShopDef = shopDef == 0;
        if (noShopDef)
            return items;
        for (std::map<long, CShopItem>::const_iterator it = Stock.begin(); it != Stock.end(); ++it) {
            if (it->first > 0 && it->second.Quantity > 0.0f) {
                std::map<long, CStockItemDef>::const_iterator d = defs.find(it->first);
                if (d != defs.end() && d->second.DefaultPrice <= shopDef->MaxPriceToDisplay && d->second.CanBeDisplayedInShop)
                    items.push_back(it->first);
            }
        }
        std::random_shuffle(items.begin(), items.end());
        return items;
    }
};
int main()
{
    CShopDef sd = { 100.0f };
    Model m; m.shopDef = 0;
    CShopItem q1 = { 1.0f, 1.0f }, q0 = { 0.0f, 1.0f };
    m.Stock[-1] = q1; m.Stock[1] = q1; m.Stock[2] = q0; m.Stock[3] = q1; m.Stock[4] = q1; m.Stock[5] = q1; m.Stock[6] = q1;
    CStockItemDef cheap = { 50, true }, dear = { 150, true }, hidden = { 10, false }, edge = { 100, true };
    m.defs[-1] = cheap; m.defs[1] = cheap; m.defs[2] = cheap; m.defs[3] = dear; m.defs[4] = hidden; m.defs[5] = edge;
    if (!m.GetSaleItemsToPlace().empty()) { std::printf("00773ee0_TEST FAIL\n"); return 1; }
    m.shopDef = &sd;
    std::vector<long> r = m.GetSaleItemsToPlace();
    std::sort(r.begin(), r.end());
    if (r.size() != 2 || r[0] != 1 || r[1] != 5) { std::printf("00773ee0_TEST FAIL\n"); return 1; }
    std::printf("00773ee0_TEST PASS\n");
    return 0;
}
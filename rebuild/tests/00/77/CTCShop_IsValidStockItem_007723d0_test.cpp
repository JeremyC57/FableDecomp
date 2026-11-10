// Behaviour test for CTCShop::IsValidStockItem (0x007723D0): true exactly when FindDefaultStockItem finds the item in the shop's
// default stock (a non-null CShopItem pointer), for every item id.
#include <cstdio>
struct CShopItem { long Id; };
class CTCShop {
public:
    const CShopItem* Stock; long StockCount;
    const CShopItem* FindDefaultStockItem(long item) const
    { for (long i = 0; i < StockCount; ++i) if (Stock[i].Id == item) return &Stock[i]; return 0; }
    bool IsValidStockItem(long item) const { return FindDefaultStockItem(item) != 0; }
};
int main()
{
    CShopItem items[3] = { { 4 }, { 9 }, { 17 } };
    CTCShop shop; shop.Stock = items; shop.StockCount = 3;
    bool ok = shop.IsValidStockItem(4) && shop.IsValidStockItem(17) && !shop.IsValidStockItem(5) && !shop.IsValidStockItem(-1);
    shop.StockCount = 0;
    ok = ok && !shop.IsValidStockItem(4);
    std::printf(ok ? "007723d0_TEST PASS\n" : "007723d0_TEST FAIL\n");
    return ok ? 0 : 1;
}

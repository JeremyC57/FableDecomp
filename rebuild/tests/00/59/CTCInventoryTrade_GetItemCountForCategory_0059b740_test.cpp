#include <cstdio>
// Behaviour of CTCInventoryTrade::GetItemCountForCategory (0x0059B740).
struct CCategory { long m_ident; long GetCategoryIdentifier() { return m_ident; } };
enum { CATEGORY_TRADE_BUY = 11, CATEGORY_TRADE_SELL = 12 };
struct CTCInventoryBase_Retail { CCategory* m_selected; CCategory* GetSelectedCategory() { return m_selected; } };
class CTCInventoryTrade : public CTCInventoryBase_Retail { public:
    long ItemSellCount; long ItemBuyCount;
    long GetItemCountForCategory()
    {
        long count = 0;
        long ident = 0;
        CCategory* category = GetSelectedCategory();
        if (category)
            ident = category->GetCategoryIdentifier();
        switch (ident) {
        case CATEGORY_TRADE_BUY: count = ItemBuyCount; break;
        case CATEGORY_TRADE_SELL: count = ItemSellCount; break;
        }
        return count;
    }
};
int main()
{
    CCategory c; CTCInventoryTrade t; t.ItemSellCount = 7; t.ItemBuyCount = 9; t.m_selected = 0;
    int bad = t.GetItemCountForCategory() != 0;
    t.m_selected = &c; c.m_ident = 11; bad |= t.GetItemCountForCategory() != 9;
    c.m_ident = 12; bad |= t.GetItemCountForCategory() != 7;
    c.m_ident = 3; bad |= t.GetItemCountForCategory() != 0;
    if (bad) { std::printf("0059b740_TEST FAIL\n"); return 1; }
    std::printf("0059b740_TEST PASS\n");
    return 0;
}
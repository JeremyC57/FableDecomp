#include <cstdio>
struct CThing { long m_id; };
struct SCategoryIdentIndex { long CategoryIdent; long CategoryIndex; };
SCategoryIdentIndex g_categoryIdentIndices[2] = { { 11, 3 }, { 12, 4 } };
struct CCategory {
    long m_index; long m_calls; long m_a; bool m_silent; long m_price; long m_count; CThing* m_thing; bool m_result;
    bool AddItem(long itemType, bool silent, long price, long count) { ++m_calls; m_a = itemType; m_silent = silent; m_price = price; m_count = count; return m_result; }
    bool AddItem(CThing* thing, bool silent, long price) { ++m_calls; m_thing = thing; m_silent = silent; m_price = price; return m_result; }
    void RemoveItem(long itemType, long count) { ++m_calls; m_a = itemType; m_count = count; }
};
static CCategory g_categories[5];
struct CTCInventoryBase_Retail { CCategory* GetCategoryWithIndex(long index) { return index < 0 ? 0 : &g_categories[index]; } };
struct CTCInventoryTrade : CTCInventoryBase_Retail {
    long CategoryIdent;
    long GetCategoryIndexForTradeType()
    {
        for (long i = 0; i < 2; ++i)
            if (g_categoryIdentIndices[i].CategoryIdent == CategoryIdent)
                return g_categoryIdentIndices[i].CategoryIndex;
        return -1;
    }
    bool AddItemOfTypeToInventory(long itemType, long count)
    {
        CCategory* category = GetCategoryWithIndex(GetCategoryIndexForTradeType());
        if (category)
            return category->AddItem(itemType, false, -1, count);
        return false;
    }
};
// Behaviour of CTCInventoryTrade::AddItemOfTypeToInventory (0x0059C6B0).
int main()
{
    CTCInventoryTrade t; t.CategoryIdent = 12; g_categories[4].m_result = true; g_categories[4].m_calls = 0;
    bool r = t.AddItemOfTypeToInventory(77, 2);
    CCategory& c = g_categories[4];
    int bad = (!r || c.m_calls != 1 || c.m_a != 77 || c.m_silent || c.m_price != -1 || c.m_count != 2);
    t.CategoryIdent = 5; bad |= t.AddItemOfTypeToInventory(1, 1);
    c.m_result = false; t.CategoryIdent = 12; bad |= t.AddItemOfTypeToInventory(1, 1);
    if (bad) { std::printf("0059c6b0_TEST FAIL\n"); return 1; }
    std::printf("0059c6b0_TEST PASS\n");
    return 0;
}

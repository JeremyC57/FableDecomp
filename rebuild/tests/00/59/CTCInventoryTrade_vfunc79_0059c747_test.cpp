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
    void vfunc_79(long itemType, long count)
    {
        CCategory* category = GetCategoryWithIndex(GetCategoryIndexForTradeType());
        if (category)
            category->RemoveItem(itemType, count);
    }
};
// Behaviour of CTCInventoryTrade::vfunc_79 (0x0059C747).
int main()
{
    CTCInventoryTrade t; t.CategoryIdent = 12; CCategory& c = g_categories[4]; c.m_calls = 0;
    t.vfunc_79(31, 6);
    int bad = (c.m_calls != 1 || c.m_a != 31 || c.m_count != 6);
    t.CategoryIdent = 1; t.vfunc_79(1, 1); bad |= (c.m_calls != 1 || g_categories[3].m_calls != 0);
    if (bad) { std::printf("0059c747_TEST FAIL\n"); return 1; }
    std::printf("0059c747_TEST PASS\n");
    return 0;
}
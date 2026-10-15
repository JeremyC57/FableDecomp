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
    bool vfunc_80(CThing* thing, bool silent, long unused0, long price, long unused1)
    {
        CCategory* category = GetCategoryWithIndex(GetCategoryIndexForTradeType());
        if (category && category->AddItem(thing, silent, price))
            return true;
        return false;
    }
};
// Behaviour of CTCInventoryTrade::vfunc_80 (0x0059C6DD).
int main()
{
    CThing th; th.m_id = 1;
    CTCInventoryTrade t; t.CategoryIdent = 11; CCategory& c = g_categories[3]; c.m_result = true; c.m_calls = 0;
    bool r = t.vfunc_80(&th, true, 99, 250, 98);
    int bad = (!r || c.m_calls != 1 || c.m_thing != &th || !c.m_silent || c.m_price != 250);
    c.m_result = false; bad |= t.vfunc_80(&th, false, 0, 1, 0);
    t.CategoryIdent = 0; bad |= t.vfunc_80(&th, false, 0, 1, 0); bad |= (c.m_calls != 2);
    if (bad) { std::printf("0059c6dd_TEST FAIL\n"); return 1; }
    std::printf("0059c6dd_TEST PASS\n");
    return 0;
}

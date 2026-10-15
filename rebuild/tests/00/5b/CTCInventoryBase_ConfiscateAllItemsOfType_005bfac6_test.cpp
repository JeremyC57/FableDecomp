#include <cstdio>
static int g_live = 0, g_conf = 0; static long g_lastType = -1;
struct CItem { long type; long n; };
struct CItemVector {
    CItem* first; CItem* last; CItem* end_of_storage; CItem buf[4];
    CItemVector() : first(0), last(0), end_of_storage(0) { ++g_live; }
    ~CItemVector() { --g_live; }
    void push(long t) { if (!first) { first = last = buf; end_of_storage = buf + 4; } last->type = t; last->n = 1; ++last; }
};
struct Inv {
    virtual void ConfiscateItems(CItemVector& items) { g_conf += (int)(items.last - items.first); if (g_live != 1) g_conf = -100; }
    void GetAllNonEmptyInventoryItems(CItemVector& items, bool includeAll) { if (includeAll) { items.push(1); items.push(2); items.push(3); } }
    void GetAllInventoryItemsOfType(long itemType, CItemVector& items) { g_lastType = itemType; items.push(itemType); }
    void ConfiscateAllItemsOfType(long itemType)
    {
        CItemVector items;
        GetAllInventoryItemsOfType(itemType, items);
        ConfiscateItems(items);
    }
};
int main()
{
    Inv inv;
    inv.ConfiscateAllItemsOfType(9);
    if (g_lastType != 9 || g_conf != 1 || g_live != 0) { std::printf("005bfac6_TEST FAIL\n"); return 1; }
    std::printf("005bfac6_TEST PASS\n");
    return 0;
}
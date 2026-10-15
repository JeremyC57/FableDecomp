#include <cstdio>
struct CItemVector { long items[16]; int n; };
struct CCategory {
    unsigned char _body[0x2C]; long stacks[2]; bool qualifies[2];
    void GetAllNonEmptyInventoryItemsInCategory(CItemVector& v, bool includeAll)
    { for (int i = 0; i < 2; ++i) if (stacks[i] != 0 && (includeAll || qualifies[i])) v.items[v.n++] = stacks[i]; }
};
struct CCategoryVector { CCategory* first; CCategory* last; CCategory* end_of_storage; };
struct Inv {
    CCategoryVector Categories;
    void GetAllNonEmptyInventoryItems(CItemVector& items, bool includeAll)
    {
        for (CCategory* it = Categories.first; it != Categories.last; ++it)
            it->GetAllNonEmptyInventoryItemsInCategory(items, includeAll);
    }
};
int main()
{
    CCategory cats[2];
    cats[0].stacks[0] = 5; cats[0].stacks[1] = 0; cats[0].qualifies[0] = false; cats[0].qualifies[1] = true;
    cats[1].stacks[0] = 7; cats[1].stacks[1] = 9; cats[1].qualifies[0] = true; cats[1].qualifies[1] = false;
    Inv inv; inv.Categories.first = cats; inv.Categories.last = cats + 2; inv.Categories.end_of_storage = cats + 2;
    CItemVector v; v.n = 0; inv.GetAllNonEmptyInventoryItems(v, true);
    if (v.n != 3 || v.items[0] != 5 || v.items[2] != 9) { std::printf("005bfa44_TEST FAIL\n"); return 1; }
    v.n = 0; inv.GetAllNonEmptyInventoryItems(v, false);
    if (v.n != 1 || v.items[0] != 7) { std::printf("005bfa44_TEST FAIL\n"); return 1; }
    std::printf("005bfa44_TEST PASS\n");
    return 0;
}
#include <cstdio>
struct CItemVector { long items[16]; int n; };
struct CCategory {
    unsigned char _body[0x2C]; long types[2];
    void GetAllInventoryItemsOfTypeInCategory(long t, CItemVector& v) { for (int i = 0; i < 2; ++i) if (types[i] == t) v.items[v.n++] = t; }
};
struct CCategoryVector { CCategory* first; CCategory* last; CCategory* end_of_storage; };
struct Inv {
    CCategoryVector Categories;
    void GetAllInventoryItemsOfType(long itemType, CItemVector& items)
    {
        for (CCategory* it = Categories.first; it != Categories.last; ++it)
            it->GetAllInventoryItemsOfTypeInCategory(itemType, items);
    }
};
int main()
{
    CCategory cats[2]; cats[0].types[0] = 4; cats[0].types[1] = 8; cats[1].types[0] = 8; cats[1].types[1] = 1;
    Inv inv; inv.Categories.first = cats; inv.Categories.last = cats + 2; inv.Categories.end_of_storage = cats + 2;
    CItemVector v; v.n = 0; inv.GetAllInventoryItemsOfType(8, v);
    if (v.n != 2) { std::printf("005bfa69_TEST FAIL\n"); return 1; }
    v.n = 0; inv.GetAllInventoryItemsOfType(3, v);
    if (v.n != 0) { std::printf("005bfa69_TEST FAIL\n"); return 1; }
    std::printf("005bfa69_TEST PASS\n");
    return 0;
}
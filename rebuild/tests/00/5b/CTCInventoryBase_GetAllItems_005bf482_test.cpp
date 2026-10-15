#include <cstdio>
struct CItemVector { long items[16]; int n; };
struct CCategory { unsigned char _body[0x2C]; long id; void GetAllItemsInCategory(CItemVector& v) { v.items[v.n++] = id; v.items[v.n++] = id * 10; } };
struct CCategoryVector { CCategory* first; CCategory* last; CCategory* end_of_storage; };
struct Inv {
    CCategoryVector Categories;
    void GetAllItems(CItemVector& items)
    {
        for (CCategory* it = Categories.first; it != Categories.last; ++it)
            it->GetAllItemsInCategory(items);
    }
};
int main()
{
    CCategory cats[3]; cats[0].id = 1; cats[1].id = 2; cats[2].id = 3;
    Inv inv; inv.Categories.first = cats; inv.Categories.last = cats + 3; inv.Categories.end_of_storage = cats + 3;
    CItemVector v; v.n = 0; inv.GetAllItems(v);
    if (v.n != 6 || v.items[0] != 1 || v.items[3] != 20 || v.items[5] != 30) { std::printf("005bf482_TEST FAIL\n"); return 1; }
    inv.Categories.last = cats; v.n = 0; inv.GetAllItems(v);
    if (v.n != 0) { std::printf("005bf482_TEST FAIL\n"); return 1; }
    std::printf("005bf482_TEST PASS\n");
    return 0;
}
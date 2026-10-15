#include <cstdio>
struct CItem { long type; long count; unsigned char _rest[0x14 - 8]; };
struct CItemVector {
    CItem* first; CItem* last; CItem* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    const CItem& operator[](unsigned long n) const { return first[n]; }
};
struct Inv {
    long total; int adds;
    void AddItem(const CItem& item) { total += item.type * item.count; ++adds; }
    void InitialiseInventoryWithItems(const CItemVector& items)
    {
        for (unsigned long i = 0; i < items.size(); ++i)
            AddItem(items[i]);
    }
};
int main()
{
    CItem it[3]; it[0].type = 1; it[0].count = 2; it[1].type = 3; it[1].count = 4; it[2].type = 5; it[2].count = 6;
    CItemVector v; v.first = it; v.last = it + 3; v.end_of_storage = it + 3;
    Inv inv; inv.total = 0; inv.adds = 0; inv.InitialiseInventoryWithItems(v);
    if (inv.adds != 3 || inv.total != 2 + 12 + 30) { std::printf("005bf8cb_TEST FAIL\n"); return 1; }
    v.last = it; inv.InitialiseInventoryWithItems(v);
    if (inv.adds != 3) { std::printf("005bf8cb_TEST FAIL\n"); return 1; }
    std::printf("005bf8cb_TEST PASS\n");
    return 0;
}

#include <cstdio>
struct CItem { long type; long count; unsigned char _rest[0x14 - 8]; };
struct CItemVector {
    CItem* first; CItem* last; CItem* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    CItem& operator[](unsigned long n) { return first[n]; }
    CItem* begin() { return first; }
    CItem* end() { return last; }
    CItem* erase(CItem* from, CItem* to) { CItem* d = from; for (CItem* s = to; s != last; ++s, ++d) *d = *s; last = d; return from; }
    void clear() { erase(begin(), end()); }
};
struct Inv {
    CItemVector ConfiscatedItems; long returned; int adds;
    void AddItem(const CItem& item) { returned += item.count; ++adds; }
    void ReturnAllConfiscatedItems()
    {
        for (unsigned long i = 0; i < ConfiscatedItems.size(); ++i)
            AddItem(ConfiscatedItems[i]);
        ConfiscatedItems.clear();
    }
};
int main()
{
    CItem it[2]; it[0].type = 1; it[0].count = 7; it[1].type = 2; it[1].count = 5;
    Inv inv; inv.returned = 0; inv.adds = 0;
    inv.ConfiscatedItems.first = it; inv.ConfiscatedItems.last = it + 2; inv.ConfiscatedItems.end_of_storage = it + 2;
    inv.ReturnAllConfiscatedItems();
    if (inv.adds != 2 || inv.returned != 12 || inv.ConfiscatedItems.size() != 0) { std::printf("005bf91a_TEST FAIL\n"); return 1; }
    inv.ReturnAllConfiscatedItems();
    if (inv.adds != 2) { std::printf("005bf91a_TEST FAIL\n"); return 1; }
    std::printf("005bf91a_TEST PASS\n");
    return 0;
}

// Behaviour test for 0x00590198 CTCInventory::ContructItemLists* (model copy: guild seal first, then the other map entries in key order)
#include <cstdio>
#include <cstring>
struct Pair { long first; long second; };
struct FakeMap {
    Pair e[8]; int n;
    int find(long k) const { for (int i = 0; i < n; ++i) if (e[i].first == k) return i; return n; }
    int begin() const { return 0; }
    int end() const { return n; }
};
static long DefIndexFromName(const char* name) { return std::strcmp(name, "OBJECT_GUILD_SEAL_1") == 0 ? 42 : -1; }
struct Inventory {
    long calls[8][2]; int ncalls;
    void ConstructItemEntry(long def, long count) { calls[ncalls][0] = def; calls[ncalls][1] = count; ++ncalls; }
    void ContructItemLists(FakeMap& items)
    {
        int seal;
        { const char* name = "OBJECT_GUILD_SEAL_1"; seal = items.find(DefIndexFromName(name)); }
        if (seal != items.end()) ConstructItemEntry(items.e[seal].first, items.e[seal].second);
        int last = items.end();
        for (int it = items.begin(); it != last; ++it)
            if (it != seal) ConstructItemEntry(items.e[it].first, items.e[it].second);
    }
};
int main()
{
    FakeMap m; m.n = 3; m.e[0].first = 7; m.e[0].second = 1; m.e[1].first = 42; m.e[1].second = 5; m.e[2].first = 99; m.e[2].second = 2;
    Inventory inv; inv.ncalls = 0;
    inv.ContructItemLists(m);
    if (inv.ncalls != 3 || inv.calls[0][0] != 42 || inv.calls[0][1] != 5 || inv.calls[1][0] != 7 || inv.calls[2][0] != 99) { std::printf("00590198_TEST FAIL\n"); return 1; }
    FakeMap m2; m2.n = 2; m2.e[0].first = 1; m2.e[0].second = 1; m2.e[1].first = 2; m2.e[1].second = 2;
    inv.ncalls = 0; inv.ContructItemLists(m2);
    if (inv.ncalls != 2 || inv.calls[0][0] != 1 || inv.calls[1][0] != 2) { std::printf("00590198_TEST FAIL\n"); return 1; }
    std::printf("00590198_TEST PASS\n");
    return 0;
}
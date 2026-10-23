// Behaviour test for CTCInventoryTrade::ConstructSpecialShopListPC @ 0x005A8A5D: total of affordable, addable, positive, not-yet-owned specials
#include <cstdio>
struct Item { long type; long count; int affordable; int canAdd; int special; int owned; };
static long Total(const Item* items, int n, int notInStockEmpty, int* wroteText)
{
    long total = 0;
    *wroteText = 0;
    if (n == 0 && notInStockEmpty) return total;
    for (int i = 0; i < n; ++i) {
        const Item& it = items[i];
        if (it.affordable && (it.canAdd || it.special) && it.count > 0 && !it.owned)
            total += it.count;
    }
    *wroteText = 1;
    return total;
}
int main()
{
    int ok = 1, wrote = 0;
    Item items[4] = { { 1, 100, 1, 1, 0, 0 }, { 2, 50, 0, 1, 0, 0 }, { 3, 30, 1, 0, 1, 0 }, { 4, 20, 1, 1, 0, 1 } };
    if (Total(items, 4, 1, &wrote) != 130 || !wrote) ok = 0;
    if (Total(items, 0, 1, &wrote) != 0 || wrote) ok = 0;
    if (Total(items, 0, 0, &wrote) != 0 || !wrote) ok = 0;
    if (!ok) { std::printf("005a8a5d_TEST FAIL\n"); return 1; }
    std::printf("005a8a5d_TEST PASS\n");
    return 0;
}

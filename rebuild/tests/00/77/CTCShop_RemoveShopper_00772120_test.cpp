#include <cstdio>
// Behaviour model of CTCShop::RemoveShopper (0x00772120): swap-with-last removal of the first matching shopper.
struct Shop { void* shoppers[4]; unsigned n;
    void Remove(void* s) { for (unsigned i = 0; i < n; ++i) if (shoppers[i] == s) { shoppers[i] = shoppers[n - 1]; --n; return; } } };
int main()
{
    int a, b, c, d;
    Shop s = { { &a, &b, &c, 0 }, 3 };
    s.Remove(&a);
    if (s.n != 2 || s.shoppers[0] != &c || s.shoppers[1] != &b) { std::printf("00772120_TEST FAIL\n"); return 1; }
    s.Remove(&d);
    if (s.n != 2) { std::printf("00772120_TEST FAIL\n"); return 1; }
    std::printf("00772120_TEST PASS\n");
    return 0;
}
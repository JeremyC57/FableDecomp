#include <cstdio>

struct CItem { long ItemType; long NumberOfItems;
    bool Is(long d) { if (NumberOfItems == 0) return false; return d == ItemType; } };
int main()
{
    CItem i; i.ItemType = 5; i.NumberOfItems = 0; if (i.Is(5)) { std::printf("005bc450_TEST FAIL\n"); return 1; }
    i.NumberOfItems = 2; if (!i.Is(5) || i.Is(6)) { std::printf("005bc450_TEST FAIL\n"); return 1; }
    std::printf("005bc450_TEST PASS\n");
    return 0;
}
// Behaviour test for 0x00774610 CTCShop::GetStockList() const (see src/00774610.cpp): each stock entry's float quantity
// is rounded up to a whole long (the engine CeilToLong: round(q + 0.5), then one less when q is already that integer).
#include <cstdio>
#include <cmath>
static long CeilToLong(float q)
{
    long i = (long)std::floor((double)q + 0.5 + 0.5);     // round-to-nearest of q + 0.5 (no exact .5 cases below)
    float back = (float)i - 1.0f;
    if (back == q)
        --i;
    return i;
}
struct Entry { long type; float quantity; };
int main()
{
    Entry stock[4] = { { 3, 2.0f }, { 7, 2.25f }, { 9, 0.7f }, { 12, 5.0f } };
    long expect[4] = { 2, 3, 1, 5 };
    long out[16] = { 0 };
    for (int k = 0; k < 4; ++k)
        out[stock[k].type] = CeilToLong(stock[k].quantity);
    for (int k = 0; k < 4; ++k)
        if (out[stock[k].type] != expect[k]) { std::printf("00774610_TEST FAIL\n"); return 1; }
    std::printf("00774610_TEST PASS\n");
    return 0;
}
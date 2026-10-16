#include <cstdio>
// Behaviour model of CTCInventoryBase::GetAllSelectedItems (0x005BEAF5): clears the out vector then appends every
// category's highlighted item in order.
struct Cat { long highlighted; long GetHighlightedItem() const { return highlighted; } };
static unsigned int Collect(const Cat* c, unsigned int n, long* out, unsigned int outCount)
{
    outCount = 0;
    for (unsigned int i = 0; i < n; ++i) out[outCount++] = c[i].GetHighlightedItem();
    return outCount;
}
int main()
{
    Cat c[3] = { { 4 }, { -1 }, { 9 } };
    long out[8] = { 1, 1, 1, 1, 1 };
    unsigned int n = Collect(c, 3, out, 5);
    if (n != 3 || out[0] != 4 || out[1] != -1 || out[2] != 9) { std::printf("005beaf5_TEST FAIL\n"); return 1; }
    if (Collect(c, 0, out, 3) != 0) { std::printf("005beaf5_TEST FAIL\n"); return 1; }
    std::printf("005beaf5_TEST PASS\n");
    return 0;
}
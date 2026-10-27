#include <cstdio>
#include <algorithm>
#include <functional>
// Behaviour model: the range ends sorted ascending by the comparator (large and small ranges).
int main()
{
    long a[40];
    for (int i = 0; i < 40; ++i) a[i] = (i * 37) % 41 - 20;
    std::sort(a, a + 40, std::less<long>());
    for (int i = 1; i < 40; ++i) if (a[i - 1] > a[i]) { std::printf("00ae7dc0_TEST FAIL\n"); return 1; }
    long b[5] = { 3, -1, 2, 2, 0 };
    std::sort(b, b + 5, std::less<long>());
    if (b[0] != -1 || b[4] != 3) { std::printf("00ae7dc0_TEST FAIL\n"); return 1; }
    std::printf("00ae7dc0_TEST PASS\n");
    return 0;
}
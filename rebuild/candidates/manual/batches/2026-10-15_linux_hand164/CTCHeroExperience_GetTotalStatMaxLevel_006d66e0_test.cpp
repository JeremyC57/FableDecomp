#include <cstdio>
// Behaviour model of CTCHeroExperience::GetTotalStatMaxLevel (0x006D66E0).
struct Track { int category; unsigned int abilities; };
static long GetTotalStatMaxLevel(const Track* t, unsigned int n, int category)
{
    long total = 0;
    for (unsigned int i = 0; i < n; ++i)
        if (t[i].category == category) { long m = (long)t[i].abilities - 1; if (m < 0) m = 0; total += m; }
    return total;
}
int main()
{
    Track t[4] = { { 1, 5 }, { 2, 3 }, { 1, 0 }, { 1, 2 } };
    if (GetTotalStatMaxLevel(t, 4, 1) != 5 || GetTotalStatMaxLevel(t, 4, 2) != 2 || GetTotalStatMaxLevel(t, 4, 3) != 0) { std::printf("006d66e0_TEST FAIL\n"); return 1; }
    std::printf("006d66e0_TEST PASS\n");
    return 0;
}

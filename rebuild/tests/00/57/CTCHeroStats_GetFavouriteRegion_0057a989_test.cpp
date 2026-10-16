#include <cstdio>
// Behaviour model of CTCHeroStats::GetFavouriteRegion (0x0057A989): the region index with the largest time wins
// (first one on ties); with no regions the out index becomes -1 and the result is the empty text.
static int Favourite(const long* times, unsigned int n, long& region)
{
    long bestTime = -1;
    long best = -1;
    for (unsigned int i = 0; i != n; ++i) {
        if (times[i] > bestTime) { bestTime = times[i]; best = (long)i; }
    }
    if (best != -1)
        return (int)best;        // text of region `best`
    region = -1;
    return -100;                 // empty text
}
int main()
{
    long t1[4] = { 3, 9, 9, 1 };
    long r = 7;
    if (Favourite(t1, 4, r) != 1 || r != 7) { std::printf("0057a989_TEST FAIL\n"); return 1; }
    if (Favourite(t1, 0, r) != -100 || r != -1) { std::printf("0057a989_TEST FAIL\n"); return 1; }
    long t2[2] = { 0, 0 };
    r = 5;
    if (Favourite(t2, 2, r) != 0) { std::printf("0057a989_TEST FAIL\n"); return 1; }
    std::printf("0057a989_TEST PASS\n");
    return 0;
}
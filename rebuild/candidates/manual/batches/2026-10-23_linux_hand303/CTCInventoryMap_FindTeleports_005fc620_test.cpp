// Behaviour test for CTCInventoryMap::FindTeleports @ 0x005FC620: a region index is recorded once per linked map that holds
// an enabled teleporter (the thing walk stops at the first enabled one); regions run 1..count inclusive
#include <cstdio>
struct Thing { int map; int enabled; };
static int Find(int regionCount, const int (*links)[3], const int* linkCount, const Thing* things, int nThings, int* out)
{
    int n = 0;
    for (int r = 1; r <= regionCount; ++r)
        for (int i = 0; i < linkCount[r]; ++i)
            for (int t = 0; t < nThings; ++t)
                if (things[t].map == links[r][i] && things[t].enabled) { out[n++] = r; break; }
    return n;
}
int main()
{
    int links[4][3] = { { 0, 0, 0 }, { 10, 11, 0 }, { 12, 0, 0 }, { 10, 12, 13 } };
    int counts[4] = { 0, 2, 1, 3 };
    Thing things[4] = { { 10, 1 }, { 10, 1 }, { 12, 0 }, { 13, 1 } };
    int out[16];
    int n = Find(3, links, counts, things, 4, out);
    int ok = n == 3 && out[0] == 1 && out[1] == 3 && out[2] == 3;
    if (Find(0, links, counts, things, 4, out) != 0) ok = 0;
    if (!ok) { std::printf("005fc620_TEST FAIL\n"); return 1; }
    std::printf("005fc620_TEST PASS\n");
    return 0;
}

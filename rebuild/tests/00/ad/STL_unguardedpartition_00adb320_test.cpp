// Behaviour test for 0x00ADB320 __unguarded_partition over CBlockInfo (Hoare partition on Cost).
#include <cstdio>

struct CBlockInfo { unsigned long BlockIndex; long Cost; long Benefit; bool operator<(const CBlockInfo& o) const { return Cost < o.Cost; } };
struct LessBlock { bool operator()(const CBlockInfo& a, const CBlockInfo& b) const { return a < b; } };

// reconstructed body
static CBlockInfo* UnguardedPartition(CBlockInfo* first, CBlockInfo* last, CBlockInfo pivot, LessBlock comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        CBlockInfo tmp = *first; *first = *last; *last = tmp;
        ++first;
    }
}

int main()
{
    CBlockInfo a[8] = { { 0, 9, 0 }, { 1, -2, 1 }, { 2, 5, 2 }, { 3, 4, 3 }, { 4, 12, 4 }, { 5, -7, 5 }, { 6, 4, 6 }, { 7, 1, 7 } };
    CBlockInfo pivot = { 99, 4, 99 };
    CBlockInfo* cut = UnguardedPartition(a, a + 8, pivot, LessBlock());
    int k = (int)(cut - a);
    if (k <= 0 || k >= 8) { std::printf("00adb320_TEST FAIL\n"); return 1; }
    for (int i = 0; i < k; ++i) if (a[i].Cost > 4) { std::printf("00adb320_TEST FAIL\n"); return 1; }
    for (int i = k; i < 8; ++i) if (a[i].Cost < 4) { std::printf("00adb320_TEST FAIL\n"); return 1; }
    // records move as whole units
    unsigned long seen = 0;
    for (int i = 0; i < 8; ++i) { if ((long)a[i].BlockIndex != a[i].Benefit) { std::printf("00adb320_TEST FAIL\n"); return 1; } seen |= 1ul << a[i].BlockIndex; }
    if (seen != 0xFFul) { std::printf("00adb320_TEST FAIL\n"); return 1; }
    std::printf("00adb320_TEST PASS\n");
    return 0;
}
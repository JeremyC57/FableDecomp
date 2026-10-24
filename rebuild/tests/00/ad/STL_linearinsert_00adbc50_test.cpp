// Behaviour test for 0x00ADBC50 __linear_insert over CBlockInfo (front insert via copy_backward, else unguarded sink).
#include <cstdio>

struct CBlockInfo { unsigned long BlockIndex; long Cost; long Benefit; bool operator<(const CBlockInfo& o) const { return Cost < o.Cost; } };
struct LessBlock { bool operator()(const CBlockInfo& a, const CBlockInfo& b) const { return a < b; } };

static int g_copyBackward = 0, g_unguarded = 0;
static CBlockInfo* CopyBackward(CBlockInfo* first, CBlockInfo* last, CBlockInfo* result)   // 0x00ADAB20
{
    ++g_copyBackward;
    for (int n = (int)(last - first); n > 0; --n) *--result = *--last;
    return result;
}
static void UnguardedLinearInsert(CBlockInfo* last, CBlockInfo val, LessBlock comp)          // 0x00ADAA30
{
    ++g_unguarded;
    CBlockInfo* next = last; --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
// reconstructed body
static void LinearInsert(CBlockInfo* first, CBlockInfo* last, CBlockInfo val, LessBlock comp)
{
    if (comp(val, *first)) { CopyBackward(first, last, last + 1); *first = val; }
    else UnguardedLinearInsert(last, val, comp);
}

int main()
{
    CBlockInfo a[4] = { { 0, 2, 0 }, { 1, 5, 1 }, { 2, 8, 2 }, { 3, -1, 3 } };
    LinearInsert(a, a + 3, a[3], LessBlock());                     // smaller than the first: shift all, goes first
    if (g_copyBackward != 1 || g_unguarded != 0 || a[0].Cost != -1 || a[0].BlockIndex != 3 || a[1].Cost != 2 || a[3].Cost != 8 || a[3].Benefit != 2) { std::printf("00adbc50_TEST FAIL\n"); return 1; }
    CBlockInfo b[4] = { { 0, 2, 0 }, { 1, 5, 1 }, { 2, 8, 2 }, { 3, 6, 3 } };
    LinearInsert(b, b + 3, b[3], LessBlock());                     // not below the first: unguarded sink
    if (g_copyBackward != 1 || g_unguarded != 1 || b[0].Cost != 2 || b[1].Cost != 5 || b[2].Cost != 6 || b[2].BlockIndex != 3 || b[3].Cost != 8) { std::printf("00adbc50_TEST FAIL\n"); return 1; }
    CBlockInfo c[2] = { { 0, 4, 0 }, { 1, 4, 1 } };
    LinearInsert(c, c + 1, c[1], LessBlock());                     // equal to the first: not less, stays
    if (g_unguarded != 2 || c[0].BlockIndex != 0 || c[1].BlockIndex != 1) { std::printf("00adbc50_TEST FAIL\n"); return 1; }
    std::printf("00adbc50_TEST PASS\n");
    return 0;
}
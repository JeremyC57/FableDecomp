// Behaviour test for 0x00ADAA30 __unguarded_linear_insert over CBlockInfo (ordered by Cost, signed).
#include <cstdio>

struct CBlockInfo { unsigned long BlockIndex; long Cost; long Benefit; bool operator<(const CBlockInfo& o) const { return Cost < o.Cost; } };
struct LessBlock { bool operator()(const CBlockInfo& a, const CBlockInfo& b) const { return a < b; } };

// reconstructed body
static void UnguardedLinearInsert(CBlockInfo* last, CBlockInfo val, LessBlock comp)
{
    CBlockInfo* next = last;
    --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}

int main()
{
    // sorted prefix {-5, 2, 7, 9}, insert Cost 3 at the end slot
    CBlockInfo a[5] = { { 0, -5, 10 }, { 1, 2, 11 }, { 2, 7, 12 }, { 3, 9, 13 }, { 4, 3, 14 } };
    UnguardedLinearInsert(a + 4, a[4], LessBlock());
    long expect[5] = { -5, 2, 3, 7, 9 };
    for (int i = 0; i < 5; ++i) if (a[i].Cost != expect[i]) { std::printf("00adaa30_TEST FAIL\n"); return 1; }
    if (a[2].BlockIndex != 4 || a[2].Benefit != 14 || a[3].BlockIndex != 2 || a[4].Benefit != 13) { std::printf("00adaa30_TEST FAIL\n"); return 1; }
    // equal key stops (stable: strict less), value already in place
    CBlockInfo b[3] = { { 0, 1, 0 }, { 1, 4, 0 }, { 2, 4, 99 } };
    UnguardedLinearInsert(b + 2, b[2], LessBlock());
    if (b[1].BlockIndex != 1 || b[2].BlockIndex != 2 || b[2].Benefit != 99) { std::printf("00adaa30_TEST FAIL\n"); return 1; }
    // guard element: a smaller value stops at the sentinel -10
    CBlockInfo c[4] = { { 9, -10, 0 }, { 1, 5, 0 }, { 2, 6, 0 }, { 3, -7, 7 } };
    UnguardedLinearInsert(c + 3, c[3], LessBlock());
    if (c[0].Cost != -10 || c[1].Cost != -7 || c[1].BlockIndex != 3 || c[2].Cost != 5 || c[3].Cost != 6) { std::printf("00adaa30_TEST FAIL\n"); return 1; }
    std::printf("00adaa30_TEST PASS\n");
    return 0;
}
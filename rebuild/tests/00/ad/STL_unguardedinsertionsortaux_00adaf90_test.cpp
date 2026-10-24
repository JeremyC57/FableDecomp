// Behaviour test for 0x00ADAF90 __unguarded_insertion_sort_aux over CBlockInfo (needs a guard minimum before first).
#include <cstdio>

struct CBlockInfo { unsigned long BlockIndex; long Cost; long Benefit; bool operator<(const CBlockInfo& o) const { return Cost < o.Cost; } };
struct LessBlock { bool operator()(const CBlockInfo& a, const CBlockInfo& b) const { return a < b; } };

static void UnguardedLinearInsert(CBlockInfo* last, CBlockInfo val, LessBlock comp)
{
    CBlockInfo* next = last;
    --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
// reconstructed body
static void UnguardedInsertionSortAux(CBlockInfo* first, CBlockInfo* last, CBlockInfo*, LessBlock comp)
{
    for (CBlockInfo* i = first; i != last; ++i)
        UnguardedLinearInsert(i, CBlockInfo(*i), comp);
}

int main()
{
    // a[0] is the already-sorted guard (minimum); sort a[1..7)
    CBlockInfo a[7] = { { 0, -100, 0 }, { 1, 50, 1 }, { 2, -3, 2 }, { 3, 50, 3 }, { 4, 0, 4 }, { 5, 7, 5 }, { 6, -3, 6 } };
    UnguardedInsertionSortAux(a + 1, a + 7, (CBlockInfo*)0, LessBlock());
    long cost[7] = { -100, -3, -3, 0, 7, 50, 50 };
    unsigned long idx[7] = { 0, 2, 6, 4, 5, 1, 3 };   // equal costs keep their order
    for (int i = 0; i < 7; ++i)
        if (a[i].Cost != cost[i] || a[i].BlockIndex != idx[i] || a[i].Benefit != (long)idx[i]) { std::printf("00adaf90_TEST FAIL\n"); return 1; }
    UnguardedInsertionSortAux(a + 3, a + 3, (CBlockInfo*)0, LessBlock());   // empty range
    if (a[3].Cost != 0) { std::printf("00adaf90_TEST FAIL\n"); return 1; }
    std::printf("00adaf90_TEST PASS\n");
    return 0;
}
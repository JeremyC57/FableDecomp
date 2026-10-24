// Behaviour test for 0x00ADBF00 __partial_sort over CBlockInfo: the first (middle - first) smallest costs end up sorted.
#include <cstdio>
#include <cstdlib>

struct CBlockInfo { unsigned long BlockIndex; long Cost; long Benefit; bool operator<(const CBlockInfo& o) const { return Cost < o.Cost; } };
struct LessBlock { bool operator()(const CBlockInfo& a, const CBlockInfo& b) const { return a < b; } };

// stand-ins for the out-of-line heap helpers (0x00ADB000 / 0x00ADB420 / 0x00ADBCB0), textbook SGI bodies
static void PushHeap(CBlockInfo* first, int hole, int top, CBlockInfo val, LessBlock comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], val)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = val;
}
static void AdjustHeap(CBlockInfo* first, int hole, int len, CBlockInfo val, LessBlock comp)
{
    int top = hole, child = 2 * hole + 2;
    while (child < len) { if (comp(first[child], first[child - 1])) child--; first[hole] = first[child]; hole = child; child = 2 * (child + 1); }
    if (child == len) { first[hole] = first[child - 1]; hole = child - 1; }
    PushHeap(first, hole, top, val, comp);
}
static void MakeHeap(CBlockInfo* first, CBlockInfo* last, LessBlock comp)
{
    int len = (int)(last - first);
    if (len < 2) return;
    for (int parent = (len - 2) / 2; ; --parent) { AdjustHeap(first, parent, len, first[parent], comp); if (parent == 0) return; }
}
static void SortHeap(CBlockInfo* first, CBlockInfo* last, LessBlock comp)
{
    while (last - first > 1) { --last; CBlockInfo v = *last; *last = *first; AdjustHeap(first, 0, (int)(last - first), v, comp); }
}
// reconstructed body
static void PartialSort(CBlockInfo* first, CBlockInfo* middle, CBlockInfo* last, CBlockInfo*, LessBlock comp)
{
    MakeHeap(first, middle, comp);
    for (CBlockInfo* i = middle; i < last; ++i)
        if (comp(*i, *first)) { CBlockInfo val = *i; *i = *first; AdjustHeap(first, 0, (int)(middle - first), val, comp); }
    SortHeap(first, middle, comp);
}

int main()
{
    const int N = 40;
    CBlockInfo a[N];
    std::srand(425);
    int histogram[201] = { 0 };
    for (int i = 0; i < N; ++i) { a[i].BlockIndex = i; a[i].Cost = std::rand() % 201 - 100; a[i].Benefit = (long)i * 3; ++histogram[a[i].Cost + 100]; }
    PartialSort(a, a + 10, a + N, (CBlockInfo*)0, LessBlock());
    for (int i = 1; i < 10; ++i) if (a[i - 1].Cost > a[i].Cost) { std::printf("00adbf00_TEST FAIL\n"); return 1; }
    for (int i = 10; i < N; ++i) if (a[i].Cost < a[9].Cost) { std::printf("00adbf00_TEST FAIL\n"); return 1; }
    for (int i = 0; i < N; ++i) { if (a[i].Benefit != (long)a[i].BlockIndex * 3) { std::printf("00adbf00_TEST FAIL\n"); return 1; } --histogram[a[i].Cost + 100]; }
    for (int i = 0; i <= 200; ++i) if (histogram[i] != 0) { std::printf("00adbf00_TEST FAIL\n"); return 1; }
    std::printf("00adbf00_TEST PASS\n");
    return 0;
}
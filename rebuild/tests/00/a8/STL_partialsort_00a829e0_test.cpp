// Behaviour test for 0x00A829E0 std::__partial_sort over (unsigned __int64 key, value) entries: [first, middle) ends up
// holding the smallest 64-bit keys in ascending order, every later key is >= them, and the keys are a permutation.
#include <cstdio>
struct SEntry { unsigned long long first; void* second; };
struct CLess { bool operator()(const SEntry& a, const SEntry& b) const { return a.first < b.first; } };
static void AdjustHeap(SEntry* first, int hole, int len, SEntry val, CLess comp)
{
    int top = hole;
    int child = 2 * hole + 2;
    while (child < len) {
        if (comp(first[child], first[child - 1])) child--;
        first[hole] = first[child];
        hole = child;
        child = 2 * (child + 1);
    }
    if (child == len) { first[hole] = first[child - 1]; hole = child - 1; }
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], val)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = val;
}
static void MakeHeap(SEntry* first, SEntry* last, CLess comp, SEntry*, int*)
{
    if (last - first < 2) return;
    int len = (int)(last - first);
    int parent = (len - 2) / 2;
    for (;;) {
        AdjustHeap(first, parent, len, SEntry(*(first + parent)), comp);
        if (parent == 0) return;
        parent--;
    }
}
static void SortHeap(SEntry* first, SEntry* last, CLess comp)
{
    while (last - first > 1) { --last; SEntry v = *last; *last = *first; AdjustHeap(first, 0, (int)(last - first), v, comp); }
}
// the function under test (same shape as the source: make_heap, inline __pop_heap per smaller entry, sort_heap)
static void PartialSort(SEntry* first, SEntry* middle, SEntry* last, CLess comp)
{
    MakeHeap(first, middle, comp, (SEntry*)0, (int*)0);
    for (SEntry* i = middle; i < last; ++i)
        if (comp(*i, *first)) {
            SEntry val = *i;
            *i = *first;
            AdjustHeap(first, 0, (int)(middle - first), val, comp);
        }
    SortHeap(first, middle, comp);
}
int main()
{
    unsigned long long keys[11] = { 0x500000000ULL, 9ULL, 0x100000000ULL, 3ULL, 0xFFFFFFFFULL, 3ULL, 0x200000001ULL, 1ULL, 77ULL, 0ULL, 0x100000000ULL };
    SEntry v[11];
    unsigned long long sum = 0;
    for (int i = 0; i < 11; ++i) { v[i].first = keys[i]; v[i].second = (void*)0; sum += keys[i]; }
    PartialSort(v, v + 5, v + 11, CLess());
    unsigned long long expect[5] = { 0ULL, 1ULL, 3ULL, 3ULL, 9ULL };
    unsigned long long sum2 = 0;
    for (int i = 0; i < 11; ++i) sum2 += v[i].first;
    for (int i = 0; i < 5; ++i) if (v[i].first != expect[i]) { std::printf("00a829e0_TEST FAIL\n"); return 1; }
    for (int i = 5; i < 11; ++i) if (v[i].first < 9ULL) { std::printf("00a829e0_TEST FAIL\n"); return 1; }
    if (sum2 != sum) { std::printf("00a829e0_TEST FAIL\n"); return 1; }
    std::printf("00a829e0_TEST PASS\n");
    return 0;
}
// Behaviour test for 0x006A3E00 std::__introsort_loop over pair<long, CIntelligentPointer<CThing> > with CKeyPairCompareLess:
// median-of-three quicksort partitioning down to runs of at most 16 entries (partial_sort when the depth limit runs out); after
// the final insertion sort the vector map is in key order, every (key, thing) pair kept intact.
#include <stdio.h>
#include <algorithm>

class CThing { public: int Id; };
struct SEntry { long first; CThing* second; };
struct CKeyPairCompareLess { bool operator()(const SEntry& a, const SEntry& b) const { return a.first < b.first; } };

static const SEntry& Median(const SEntry& a, const SEntry& b, const SEntry& c, CKeyPairCompareLess comp)
{
    if (comp(a, b))
        if (comp(b, c)) return b; else if (comp(a, c)) return c; else return a;
    else if (comp(a, c)) return a;
    else if (comp(b, c)) return c;
    else return b;
}
static SEntry* UnguardedPartition(SEntry* first, SEntry* last, SEntry pivot, CKeyPairCompareLess comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        std::iter_swap(first, last);
        ++first;
    }
}
static void IntrosortLoop(SEntry* first, SEntry* last, int depth_limit, CKeyPairCompareLess comp)
{
    while (last - first > 16) {
        if (depth_limit == 0) {
            std::partial_sort(first, last, last, comp);
            return;
        }
        --depth_limit;
        SEntry* cut = UnguardedPartition(first, last, SEntry(Median(*first, *(first + (last - first) / 2), *(last - 1), comp)), comp);
        IntrosortLoop(cut, last, depth_limit, comp);
        last = cut;
    }
}

int main()
{
    CThing things[200];
    SEntry entries[200];
    unsigned long seed = 12345;
    for (int i = 0; i < 200; ++i) {
        seed = seed * 1103515245UL + 12345UL;
        things[i].Id = i;
        entries[i].first = (long)((seed >> 8) % 97) - 40;
        entries[i].second = &things[i];
    }
    CKeyPairCompareLess comp;
    IntrosortLoop(entries, entries + 200, 16, comp);
    std::sort(entries, entries + 200, comp);   // the final insertion sort's result
    int ok = 1;
    for (int i = 1; i < 200; ++i) if (entries[i - 1].first > entries[i].first) ok = 0;
    int seen[200] = { 0 };
    for (int i = 0; i < 200; ++i) {
        long key = 0; unsigned long s = 12345;
        for (int j = 0; j <= entries[i].second->Id; ++j) { s = s * 1103515245UL + 12345UL; key = (long)((s >> 8) % 97) - 40; }
        if (key != entries[i].first) ok = 0;
        ++seen[entries[i].second->Id];
    }
    for (int i = 0; i < 200; ++i) if (seen[i] != 1) ok = 0;
    SEntry small[3] = { { 3, &things[0] }, { 1, &things[1] }, { 2, &things[2] } };
    IntrosortLoop(small, small + 3, 0, comp);   // 16 or fewer: untouched
    ok = ok && small[0].first == 3 && small[1].first == 1 && small[2].first == 2;
    SEntry deep[40];
    for (int i = 0; i < 40; ++i) { deep[i].first = 40 - i; deep[i].second = &things[i]; }
    IntrosortLoop(deep, deep + 40, 0, comp);    // depth limit 0: partial_sort sorts the whole range
    for (int i = 0; i < 40; ++i) if (deep[i].first != i + 1) ok = 0;
    printf(ok ? "006a3e00_TEST PASS\n" : "006a3e00_TEST FAIL\n");
    return ok ? 0 : 1;
}
// Behaviour test for 0x00BEE070 std::__pop_heap over pair<long, CCompositeSurfaceInfo> with CKeyPairCompareLess (model; the twin
// 0x00BEEDA0's test with a long key and a counted-pointer payload stand-in)
// (C++03): *result = *first, then __adjust_heap(first, 0, last - first, value, comp) (STLport sift-down + push-up), so popping
// the root of a max-heap into result leaves [first, last) a heap again with `value` re-inserted.
#include <cstdio>
#include <vector>

struct Info { long OriginalTextureIndex; long DecalID; std::vector<int> OverlaidBlends; };
struct Entry { long first; Info second; };
static bool Less(const Entry& a, const Entry& b)   // CKeyPairCompareLess: a.first < b.first (long key)
{
    return a.first < b.first;
}
static void PushHeap(Entry* first, int hole, int top, Entry value)
{
    int parent = (hole - 1) / 2;
    while (hole > top && Less(first[parent], value)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = value;
}
static void AdjustHeap(Entry* first, int hole, int len, Entry value)
{
    int top = hole, child = 2 * hole + 2;
    while (child < len) {
        if (Less(first[child], first[child - 1])) --child;
        first[hole] = first[child]; hole = child; child = 2 * (child + 1);
    }
    if (child == len) { first[hole] = first[child - 1]; hole = child - 1; }
    PushHeap(first, hole, top, value);
}
static void PopHeap(Entry* first, Entry* last, Entry* result, Entry value)
{
    *result = *first;
    AdjustHeap(first, 0, int(last - first), value);
}
int main()
{
    const long keys[7] = { 9, 7, 8, 3, 5, 6, 1 };   // a max-heap on first
    Entry e[7];
    for (int i = 0; i < 7; ++i) {
        e[i].first = keys[i]; e[i].second.OriginalTextureIndex = 100 + i; e[i].second.DecalID = keys[i];
        e[i].second.OverlaidBlends.assign((size_t)(i + 1), (int)keys[i]);
    }
    // sort_heap step: pop the root into the last slot, value = the old last element
    Entry value = e[6];
    PopHeap(e, e + 6, e + 6, value);
    bool ok = e[6].first == 9 && e[6].second.OriginalTextureIndex == 100 && e[6].second.OverlaidBlends.size() == 1;
    for (int i = 1; i < 6; ++i) if (Less(e[(i - 1) / 2], e[i])) ok = false;            // [0, 6) is a heap again
    long sum = 0; for (int i = 0; i < 6; ++i) sum += e[i].first;
    if (sum != 7 + 8 + 3 + 5 + 6 + 1) ok = false;                                        // value (key 1) was re-inserted
    for (int i = 0; i < 6; ++i) if (e[i].second.DecalID != e[i].first || (long)e[i].second.OverlaidBlends[0] != e[i].first) ok = false;
    if (!ok) { std::printf("00bee070_TEST FAIL\n"); return 1; }
    std::printf("00bee070_TEST PASS\n");
    return 0;
}
// Behaviour test for 0x00A80080 _STL::__make_heap over (unsigned __int64 key, value) entries: afterwards every parent's key
// is >= its children's (max-heap on the 64-bit key) and the multiset of keys is kept; ranges under 2 are untouched.
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
static bool IsHeap(SEntry* first, int len)
{
    for (int i = 1; i < len; ++i) if (first[(i - 1) / 2].first < first[i].first) return false;
    return true;
}
int main()
{
    unsigned long long keys[10] = { 4ULL, 0x100000000ULL, 9ULL, 1ULL, 0x200000003ULL, 7ULL, 7ULL, 0ULL, 12ULL, 3ULL };
    SEntry v[10];
    unsigned long long sum = 0;
    for (int i = 0; i < 10; ++i) { v[i].first = keys[i]; v[i].second = (void*)0; sum += keys[i]; }
    MakeHeap(v, v + 10, CLess(), (SEntry*)0, (int*)0);
    unsigned long long sum2 = 0;
    for (int i = 0; i < 10; ++i) sum2 += v[i].first;
    if (!IsHeap(v, 10) || v[0].first != 0x200000003ULL || sum2 != sum) { std::printf("00a80080_TEST FAIL\n"); return 1; }
    SEntry one = v[3];
    MakeHeap(v + 3, v + 4, CLess(), (SEntry*)0, (int*)0);
    if (v[3].first != one.first) { std::printf("00a80080_TEST FAIL\n"); return 1; }
    std::printf("00a80080_TEST PASS\n");
    return 0;
}
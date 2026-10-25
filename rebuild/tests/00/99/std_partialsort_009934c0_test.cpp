// Behaviour test for 0x009934C0 std::__partial_sort over (long key, record) entries (STLport heap-based partial sort).
#include <cstdio>

struct Entry { long first; int payload; };
struct Less { bool operator()(const Entry& a, const Entry& b) const { return a.first < b.first; } };

static void AdjustHeap(Entry* first, int hole, int len, Entry value, Less comp)
{
    int top = hole;
    int second = 2 * hole + 2;
    while (second < len) {
        if (comp(first[second], first[second - 1])) second--;
        first[hole] = first[second];
        hole = second;
        second = 2 * (second + 1);
    }
    if (second == len) { first[hole] = first[second - 1]; hole = second - 1; }
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = value;
}
static void MakeHeap(Entry* first, Entry* last, Less comp)
{
    if (last - first < 2) return;
    int len = (int)(last - first);
    int parent = (len - 2) / 2;
    for (;;) { AdjustHeap(first, parent, len, first[parent], comp); if (parent == 0) return; parent--; }
}
static void PopHeap(Entry* first, Entry* last, Entry* result, Entry val, Less comp)
{
    *result = *first;
    AdjustHeap(first, 0, (int)(last - first), val, comp);
}
static void SortHeap(Entry* first, Entry* last, Less comp)
{
    while (last - first > 1) { --last; PopHeap(first, last, last, *last, comp); }
}
static int g_partialSorts = 0;
// the 0x009934C0 body
static void PartialSort(Entry* first, Entry* middle, Entry* last, Less comp)
{
    ++g_partialSorts;
    MakeHeap(first, middle, comp);
    for (Entry* i = middle; i < last; ++i)
        if (comp(*i, *first))
            PopHeap(first, middle, i, *i, comp);
    SortHeap(first, middle, comp);
}
static void Fill(Entry* e, int n) { for (int i = 0; i < n; ++i) { e[i].first = (long)((i * 53 + 17) % 97) - 40; e[i].payload = i; } }
static bool Sorted(const Entry* e, int n) { for (int i = 1; i < n; ++i) if (e[i - 1].first > e[i].first) return false; return true; }

int main()
{
    Entry e[60];
    Fill(e, 60);
    Less comp;
    PartialSort(e, e + 10, e + 60, comp);           // the 10 smallest keys, in order, at the front
    if (!Sorted(e, 10)) { std::printf("009934c0_TEST FAIL\n"); return 1; }
    for (int i = 10; i < 60; ++i) if (e[i].first < e[9].first) { std::printf("009934c0_TEST FAIL\n"); return 1; }
    Fill(e, 60);
    PartialSort(e, e + 60, e + 60, comp);           // middle == last: a full heap sort
    if (!Sorted(e, 60)) { std::printf("009934c0_TEST FAIL\n"); return 1; }
    std::printf("009934c0_TEST PASS\n");
    return 0;
}
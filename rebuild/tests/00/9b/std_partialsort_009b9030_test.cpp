// Behaviour test for 0x009B9030 __partial_sort over (key, class info) pairs with a key-less comparator.
#include <cstdio>

struct Pair { unsigned int first; unsigned int second; };
struct KeyLess { bool operator()(const Pair& a, const Pair& b) const { return a.first < b.first; } };

static void AdjustHeap(Pair* first, int hole, int len, Pair val, KeyLess comp)
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
    while (hole > top && comp(first[parent], val)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = val;
}
static void MakeHeap(Pair* first, Pair* last, KeyLess comp)
{
    int len = (int)(last - first);
    if (len < 2) return;
    for (int parent = (len - 2) / 2; ; --parent) {
        AdjustHeap(first, parent, len, first[parent], comp);
        if (parent == 0) return;
    }
}
static void PopHeap(Pair* first, Pair* last, Pair* result, Pair val, KeyLess comp)
{
    *result = *first;
    AdjustHeap(first, 0, (int)(last - first), val, comp);
}
static void SortHeap(Pair* first, Pair* last, KeyLess comp)
{
    while (last - first > 1) { --last; PopHeap(first, last, last, *last, comp); }
}

// the row: __partial_sort(first, middle, last, (Pair*)0, comp)
static void PartialSort(Pair* first, Pair* middle, Pair* last, Pair*, KeyLess comp)
{
    MakeHeap(first, middle, comp);
    for (Pair* i = middle; i < last; ++i)
        if (comp(*i, *first))
            PopHeap(first, middle, i, Pair(*i), comp);
    SortHeap(first, middle, comp);
}

int main()
{
    unsigned int keys[9] = { 42, 7, 19, 3, 88, 1, 56, 23, 5 };
    Pair a[9];
    for (int i = 0; i < 9; ++i) { a[i].first = keys[i]; a[i].second = keys[i] * 3; }
    PartialSort(a, a + 4, a + 9, (Pair*)0, KeyLess());
    unsigned int want[4] = { 1, 3, 5, 7 };
    int ok = 1;
    for (int i = 0; i < 4; ++i)
        if (a[i].first != want[i] || a[i].second != want[i] * 3) ok = 0;
    unsigned int sum = 0;
    for (int i = 0; i < 9; ++i) sum += a[i].first;
    if (sum != 244) ok = 0;                     // the tail still holds the other keys
    if (!ok) { std::printf("009b9030_TEST FAIL\n"); return 1; }
    std::printf("009b9030_TEST PASS\n");
    return 0;
}
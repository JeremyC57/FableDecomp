// Behaviour test for 0x009E7B40 STLport __partial_sort: the first (middle-first) slots hold the smallest keys in order.
#include <cstdio>

struct SEntry { int key; };
struct CCompare { bool operator()(const SEntry& a, const SEntry& b) const { return a.key < b.key; } };

static void __adjust_heap(SEntry* first, int holeIndex, int len, SEntry value, CCompare comp)
{
    int topIndex = holeIndex;
    int secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(first[secondChild], first[secondChild - 1])) secondChild--;
        first[holeIndex] = first[secondChild];
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) { first[holeIndex] = first[secondChild - 1]; holeIndex = secondChild - 1; }
    int parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(first[parent], value)) { first[holeIndex] = first[parent]; holeIndex = parent; parent = (holeIndex - 1) / 2; }
    first[holeIndex] = value;
}
static void __make_heap(SEntry* first, SEntry* last, CCompare comp, SEntry*, int*)
{
    if (last - first < 2) return;
    int len = (int)(last - first);
    int parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, SEntry(*(first + parent)), comp);
        if (parent == 0) return;
        parent--;
    }
}
static void make_heap(SEntry* first, SEntry* last, CCompare comp) { __make_heap(first, last, comp, (SEntry*)0, (int*)0); }
static void __pop_heap(SEntry* first, SEntry* last, SEntry* result, SEntry value, CCompare comp, int*)
{
    *result = *first;
    __adjust_heap(first, 0, (int)(last - first), value, comp);
}
static void sort_heap(SEntry* first, SEntry* last, CCompare comp)
{
    while (last - first > 1) { SEntry* l = last - 1; __pop_heap(first, l, l, SEntry(*l), comp, (int*)0); last = l; }
}
static void __partial_sort(SEntry* first, SEntry* middle, SEntry* last, SEntry*, CCompare comp)
{
    make_heap(first, middle, comp);
    for (SEntry* i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, SEntry(*i), comp, (int*)0);
    sort_heap(first, middle, comp);
}

int main()
{
    int keys[12] = { 11, 4, 9, 0, 7, 2, 10, 5, 1, 8, 3, 6 };
    SEntry e[12];
    for (int i = 0; i < 12; ++i) e[i].key = keys[i];
    __partial_sort(e, e + 5, e + 12, (SEntry*)0, CCompare());
    bool ok = true;
    for (int i = 0; i < 5; ++i) ok = ok && e[i].key == i;
    int sum = 0;
    for (int i = 0; i < 12; ++i) sum += e[i].key;
    ok = ok && sum == 66;
    for (int i = 5; i < 12; ++i) ok = ok && e[i].key >= 5;
    if (!ok) { std::printf("009e7b40_TEST FAIL\n"); return 1; }
    std::printf("009e7b40_TEST PASS\n");
    return 0;
}
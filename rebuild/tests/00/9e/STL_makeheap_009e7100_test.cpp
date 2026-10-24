// Behaviour test for 0x009E7100 STLport __make_heap (with a reference __adjust_heap): the result is a max-heap.
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

static int gAdjusts = 0;
static void __make_heap(SEntry* first, SEntry* last, CCompare comp, SEntry*, int*)
{
    if (last - first < 2) return;
    int len = (int)(last - first);
    int parent = (len - 2) / 2;
    for (;;) {
        ++gAdjusts;
        __adjust_heap(first, parent, len, SEntry(*(first + parent)), comp);
        if (parent == 0) return;
        parent--;
    }
}

int main()
{
    int keys[10] = { 3, 9, 1, 7, 5, 8, 2, 6, 4, 0 };
    SEntry e[10];
    for (int i = 0; i < 10; ++i) e[i].key = keys[i];
    __make_heap(e, e + 10, CCompare(), (SEntry*)0, (int*)0);
    bool ok = gAdjusts == 5 && e[0].key == 9;
    for (int i = 1; i < 10; ++i) ok = ok && e[(i - 1) / 2].key >= e[i].key;
    SEntry one[1] = { { 42 } };
    __make_heap(one, one + 1, CCompare(), (SEntry*)0, (int*)0);
    ok = ok && gAdjusts == 5 && one[0].key == 42;
    if (!ok) { std::printf("009e7100_TEST FAIL\n"); return 1; }
    std::printf("009e7100_TEST PASS\n");
    return 0;
}
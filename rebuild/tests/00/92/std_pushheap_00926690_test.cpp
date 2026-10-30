// behaviour test for 0x00926690 std::__push_heap<CIntelligentPointer<const CThing>*, int, int, ..., compare> (STLport heap sift-up)
#include <cstdio>
struct Item { int key; };
struct Ptr {
    Item* item;
    Ptr() : item(0) { }
    Ptr(const Ptr& p) { Item* i = p.item; item = i; }
    Ptr& operator=(const Ptr& p) { Item* i = p.item; item = i; return *this; }
};
typedef bool (*Compare)(Ptr, Ptr);
static bool Less(Ptr a, Ptr b) { return a.item->key < b.item->key; }
static void push_heap_core(Ptr* first, int holeIndex, int topIndex, Ptr val, Compare comp)
{
    int parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), val)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = val;
}
int main()
{
    Item items[7] = { {9}, {5}, {8}, {1}, {4}, {7}, {10} };
    Ptr heap[7];
    int n = 0;
    bool ok = true;
    for (int i = 0; i < 7; ++i) {
        Ptr v; v.item = &items[i];
        push_heap_core(heap, n, 0, v, Less);
        ++n;
        for (int k = 1; k < n; ++k)
            ok = ok && heap[(k - 1) / 2].item->key >= heap[k].item->key;
    }
    ok = ok && heap[0].item->key == 10;
    std::printf(ok ? "00926690_TEST PASS\n" : "00926690_TEST FAIL\n");
    return ok ? 0 : 1;
}
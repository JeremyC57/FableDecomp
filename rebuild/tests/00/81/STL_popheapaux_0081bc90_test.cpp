// Behaviour test: std::__pop_heap_aux over (long key, 36-byte payload) pairs -- the largest moves to the back, the rest stays a heap
#include <cstdio>

struct SPayload {                                       // 36-byte POD payload
    unsigned int w[9];
};
struct SPair {                                          // pair<long, payload>
    long first;
    SPayload second;
    SPair() { }
    SPair(const SPair& o) : first(o.first), second(o.second) { }
};
struct CKeyPairCompareLess {
    bool operator()(const SPair& a, const SPair& b) const { return a.first < b.first; }
};

static SPair Make(long key, unsigned int tag)
{
    SPair p;
    p.first = key;
    for (int i = 0; i < 9; ++i)
        p.second.w[i] = tag * 100 + i;
    return p;
}

// STLport helpers (the out-of-line callees of the retail row, as real bodies)
static void __push_heap(SPair* first, int holeIndex, int topIndex, SPair val, CKeyPairCompareLess comp)
{
    int parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), val)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = val;
}
static void __adjust_heap(SPair* first, int holeIndex, int len, SPair val, CKeyPairCompareLess comp)
{
    int topIndex = holeIndex;
    int secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(*(first + secondChild), *(first + (secondChild - 1))))
            secondChild--;
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) {
        *(first + holeIndex) = *(first + (secondChild - 1));
        holeIndex = secondChild - 1;
    }
    __push_heap(first, holeIndex, topIndex, val, comp);
}
static void make_heap(SPair* first, SPair* last, CKeyPairCompareLess comp)
{
    if (last - first < 2)
        return;
    int len = last - first;
    int parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, SPair(*(first + parent)), comp);
        if (parent == 0)
            return;
        parent--;
    }
}
static void __pop_heap(SPair* first, SPair* last, SPair* result, SPair val, CKeyPairCompareLess comp)
{
    *result = *first;
    __adjust_heap(first, 0, int(last - first), val, comp);
}
static void sort_heap(SPair* first, SPair* last, CKeyPairCompareLess comp)
{
    while (last - first > 1) {
        --last;
        __pop_heap(first, last, last, SPair(*last), comp);
    }
}
static bool IsHeap(const SPair* a, int n)
{
    for (int i = 1; i < n; ++i)
        if (a[(i - 1) / 2].first < a[i].first)
            return false;
    return true;
}
static bool PayloadIntact(const SPair* a, int n, const long* keys)
{
    // every element keeps the payload that belongs to its key (tag = index of the key in `keys`)
    for (int i = 0; i < n; ++i) {
        int tag = -1;
        for (int k = 0; k < n; ++k)
            if (keys[k] == a[i].first)
                tag = k;
        if (tag < 0 || a[i].second.w[0] != (unsigned int)(tag * 100) || a[i].second.w[8] != (unsigned int)(tag * 100 + 8))
            return false;
    }
    return true;
}

// the row under test: STLport __pop_heap_aux (as the source)
static void __pop_heap_aux(SPair* first, SPair* last, SPair*, CKeyPairCompareLess comp)
{
    __pop_heap(first, last - 1, last - 1, SPair(*(last - 1)), comp);
}

int main()
{
    long keys[10] = { 17, -4, 33, 8, 0, 25, -11, 42, 5, 19 };
    SPair a[10];
    for (int i = 0; i < 10; ++i)
        a[i] = Make(keys[i], i);
    make_heap(a, a + 10, CKeyPairCompareLess());
    long largest = keys[0];
    for (int k = 1; k < 10; ++k)
        if (largest < keys[k])
            largest = keys[k];
    __pop_heap_aux(a, a + 10, (SPair*)0, CKeyPairCompareLess());
    if (a[9].first != largest || !IsHeap(a, 9) || !PayloadIntact(a, 10, keys))
        { std::printf("0081bc90_TEST FAIL\n"); return 1; }
    std::printf("0081bc90_TEST PASS\n");
    return 0;
}
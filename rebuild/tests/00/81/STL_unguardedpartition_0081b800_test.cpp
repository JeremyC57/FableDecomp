// Behaviour test: _STL::__unguarded_partition over (long key, 36-byte payload) pairs -- everything left of the cut is <= the pivot, right of it >= it
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

// the row under test: STLport __unguarded_partition (as the source)
static void iter_swap(SPair* a, SPair* b)
{
    SPair tmp = *a;
    *a = *b;
    *b = tmp;
}
static SPair* __unguarded_partition(SPair* first, SPair* last, SPair pivot, CKeyPairCompareLess comp)
{
    for (;;) {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        iter_swap(first, last);
        ++first;
    }
}

int main()
{
    long keys[10] = { 17, -4, 33, 8, 0, 25, -11, 42, 5, 19 };
    SPair a[10];
    for (int i = 0; i < 10; ++i)
        a[i] = Make(keys[i], i);
    SPair pivot = Make(10, 99);
    SPair* cut = __unguarded_partition(a, a + 10, pivot, CKeyPairCompareLess());
    if (cut <= a || cut >= a + 10)
        { std::printf("0081b800_TEST FAIL\n"); return 1; }
    for (SPair* p = a; p < cut; ++p)
        if (10 < p->first)
            { std::printf("0081b800_TEST FAIL\n"); return 1; }
    for (SPair* q = cut; q < a + 10; ++q)
        if (q->first < 10)
            { std::printf("0081b800_TEST FAIL\n"); return 1; }
    if (!PayloadIntact(a, 10, keys))
        { std::printf("0081b800_TEST FAIL\n"); return 1; }
    std::printf("0081b800_TEST PASS\n");
    return 0;
}
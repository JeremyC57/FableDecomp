// Behaviour test: _STL::__linear_insert over (unsigned key, 8-byte payload) pairs -- insert the last element into the sorted prefix
#include <cstdio>

struct SPayload {                                       // 8-byte POD payload
    unsigned int w[2];
};
struct SPair {                                          // pair<unsigned int, payload>
    unsigned int first;
    SPayload second;
    SPair() { }
    SPair(const SPair& o) : first(o.first), second(o.second) { }
};
struct CKeyPairCompareLess {
    bool operator()(const SPair& a, const SPair& b) const { return a.first < b.first; }
};

static SPair Make(unsigned int key, unsigned int tag)
{
    SPair p;
    p.first = key;
    for (int i = 0; i < 2; ++i)
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
static bool PayloadIntact(const SPair* a, int n, const unsigned int* keys)
{
    // every element keeps the payload that belongs to its key (tag = index of the key in `keys`)
    for (int i = 0; i < n; ++i) {
        int tag = -1;
        for (int k = 0; k < n; ++k)
            if (keys[k] == a[i].first)
                tag = k;
        if (tag < 0 || a[i].second.w[0] != (unsigned int)(tag * 100) || a[i].second.w[1] != (unsigned int)(tag * 100 + 1))
            return false;
    }
    return true;
}

// the row under test: STLport __linear_insert (as the source; the callees as real bodies)
static SPair* copy_backward(SPair* first, SPair* last, SPair* result)
{
    for (int n = int(last - first); n > 0; --n)
        *--result = *--last;
    return result;
}
static void __unguarded_linear_insert(SPair* last, SPair val, CKeyPairCompareLess comp)
{
    SPair* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}
static void __linear_insert(SPair* first, SPair* last, SPair val, CKeyPairCompareLess comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}

int main()
{
    unsigned int keys[5] = { 3, 5, 9, 12, 7 };
    SPair a[5];
    for (int i = 0; i < 5; ++i)
        a[i] = Make(keys[i], i);
    __linear_insert(a, a + 4, SPair(a[4]), CKeyPairCompareLess());       // 7 goes between 5 and 9
    if (a[0].first != 3 || a[1].first != 5 || a[2].first != 7 || a[3].first != 9 || a[4].first != 12 || !PayloadIntact(a, 5, keys))
        { std::printf("0081be80_TEST FAIL\n"); return 1; }
    unsigned int keys2[4] = { 4, 6, 8, 1 };
    SPair b[4];
    for (int i = 0; i < 4; ++i)
        b[i] = Make(keys2[i], i);
    __linear_insert(b, b + 3, SPair(b[3]), CKeyPairCompareLess());       // 1 is the new front: the copy_backward arm
    if (b[0].first != 1 || b[1].first != 4 || b[2].first != 6 || b[3].first != 8 || !PayloadIntact(b, 4, keys2))
        { std::printf("0081be80_TEST FAIL\n"); return 1; }
    std::printf("0081be80_TEST PASS\n");
    return 0;
}
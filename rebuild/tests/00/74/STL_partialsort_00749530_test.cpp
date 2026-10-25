// Behaviour test for 0x00749530 std::__partial_sort(CThing**, CThing**, CThing**, CThing**, CCompare_FurthestFromInitPos2D)
#include <cstdio>

struct CThing { int key; };

struct CCompare_FurthestFromInitPos2D {
    bool operator()(CThing* a, CThing* b) const { return a->key > b->key; }   // stand-in ordering: larger key first
};

// stand-ins for the out-of-line STLport heap helpers the retail body calls
void __push_heap(CThing** first, int holeIndex, int topIndex, CThing* val, CCompare_FurthestFromInitPos2D comp)
{
    int parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), val)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = val;
}
void __adjust_heap(CThing** first, int holeIndex, int len, CThing* val, CCompare_FurthestFromInitPos2D comp)
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
void __make_heap(CThing** first, CThing** last, CCompare_FurthestFromInitPos2D comp, CThing**, int*)
{
    int len = (int)(last - first);
    if (len < 2)
        return;
    int parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, *(first + parent), comp);
        if (parent == 0)
            return;
        parent--;
    }
}
void sort_heap(CThing** first, CThing** last, CCompare_FurthestFromInitPos2D comp)
{
    while (last - first > 1) {
        --last;
        CThing* val = *last;
        *last = *first;
        __adjust_heap(first, 0, (int)(last - first), val, comp);
    }
}
inline void make_heap(CThing** first, CThing** last, CCompare_FurthestFromInitPos2D comp)
{
    __make_heap(first, last, comp, (CThing**)0, (int*)0);
}
template <class RandomAccessIter, class T, class Compare, class Distance>
inline void __pop_heap(RandomAccessIter first, RandomAccessIter last, RandomAccessIter result, T val, Compare comp, Distance*)
{
    *result = *first;
    __adjust_heap(first, Distance(0), Distance(last - first), val, comp);
}

void __partial_sort(CThing** first, CThing** middle, CThing** last, CThing**, CCompare_FurthestFromInitPos2D comp)
{
    make_heap(first, middle, comp);
    for (CThing** i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, *i, comp, (int*)0);
    sort_heap(first, middle, comp);
}

int main()
{
    CThing t[9];
    int keys[9] = { 4, 9, 1, 15, 12, 0, 5, 7, 3 };
    CThing* p[9];
    for (int i = 0; i < 9; ++i) { t[i].key = keys[i]; p[i] = &t[i]; }
    __partial_sort(p, p + 4, p + 9, (CThing**)0, CCompare_FurthestFromInitPos2D());
    int want[4] = { 15, 12, 9, 7 };
    int ok = 1;
    for (int i = 0; i < 4; ++i) if (p[i]->key != want[i]) ok = 0;
    // the rest is a permutation of the remaining keys
    int seen = 0;
    for (int i = 4; i < 9; ++i) { int k = p[i]->key; if (k == 4 || k == 1 || k == 0 || k == 5 || k == 3) seen |= 1 << k; }
    if (seen != ((1 << 4) | (1 << 1) | (1 << 0) | (1 << 5) | (1 << 3))) ok = 0;
    // middle == last: a full heap sort
    __partial_sort(p, p + 9, p + 9, (CThing**)0, CCompare_FurthestFromInitPos2D());
    int full[9] = { 15, 12, 9, 7, 5, 4, 3, 1, 0 };
    for (int i = 0; i < 9; ++i) if (p[i]->key != full[i]) ok = 0;
    if (!ok) { std::printf("00749530_TEST FAIL\n"); return 1; }
    std::printf("00749530_TEST PASS\n");
    return 0;
}
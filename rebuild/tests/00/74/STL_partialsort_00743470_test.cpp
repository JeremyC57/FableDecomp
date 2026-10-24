// Behaviour test for 0x00743470 _STL::__partial_sort over the mode-registry pairs (unsigned key)
#include <cstdio>
struct CRegisteredModeInfo { int tag; };

template <class A, class B> struct pair {
    A first;
    B second;
    pair() {}
    pair(const pair& other) : first(other.first), second(other.second) {}
};

typedef pair<unsigned int, CRegisteredModeInfo> TModePair;   // unsigned int: the 32-bit unsigned key

struct TModeLess {
    bool operator()(const TModePair& a, const TModePair& b) const { return a.first < b.first; }
};

static void Fill(TModePair* p, const unsigned int* keys, int n)
{
    for (int i = 0; i < n; ++i) { p[i].first = keys[i]; p[i].second.tag = i; }
}

// stand-ins for the out-of-line STLport heap helpers the retail body calls
void __push_heap(TModePair* first, int holeIndex, int topIndex, TModePair val, TModeLess comp)
{
    int parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), val)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = val;
}
void __adjust_heap(TModePair* first, int holeIndex, int len, TModePair val, TModeLess comp)
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
void __make_heap(TModePair* first, TModePair* last, TModeLess comp, TModePair*, int*)
{
    int len = (int)(last - first);
    if (len < 2)
        return;
    int parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, TModePair(*(first + parent)), comp);
        if (parent == 0)
            return;
        parent--;
    }
}
void sort_heap(TModePair* first, TModePair* last, TModeLess comp)
{
    while (last - first > 1) {
        --last;
        TModePair val(*last);
        *last = *first;
        __adjust_heap(first, 0, (int)(last - first), val, comp);
    }
}
inline void make_heap(TModePair* first, TModePair* last, TModeLess comp)
{
    __make_heap(first, last, comp, (TModePair*)0, (int*)0);
}
template <class RandomAccessIter, class T, class Compare, class Distance>
inline void __pop_heap(RandomAccessIter first, RandomAccessIter last, RandomAccessIter result, T val, Compare comp, Distance*)
{
    *result = *first;
    __adjust_heap(first, Distance(0), Distance(last - first), val, comp);
}

void __partial_sort(TModePair* first, TModePair* middle, TModePair* last, TModePair*, TModeLess comp)
{
    make_heap(first, middle, comp);
    for (TModePair* i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, TModePair(*i), comp, (int*)0);
    sort_heap(first, middle, comp);
}

int main()
{
    TModePair p[9];
    unsigned int keys[9] = { 40u, 0x80000000u, 10u, 150u, 12u, 0u, 50u, 7u, 30u };
    Fill(p, keys, 9);
    __partial_sort(p, p + 4, p + 9, (TModePair*)0, TModeLess());
    unsigned int want[4] = { 0u, 7u, 10u, 12u };
    int ok = 1;
    for (int i = 0; i < 4; ++i) if (p[i].first != want[i]) ok = 0;
    if (p[0].second.tag != 5 || p[1].second.tag != 7) ok = 0;       // the values travel with their keys
    // the tail holds the other five keys
    unsigned int sum = 0;
    for (int i = 4; i < 9; ++i) sum += p[i].first;
    if (sum != 40u + 0x80000000u + 150u + 50u + 30u) ok = 0;
    // middle == last: a full heap sort, the large unsigned key last
    __partial_sort(p, p + 9, p + 9, (TModePair*)0, TModeLess());
    unsigned int full[9] = { 0u, 7u, 10u, 12u, 30u, 40u, 50u, 150u, 0x80000000u };
    for (int i = 0; i < 9; ++i) if (p[i].first != full[i]) ok = 0;
    if (!ok) { std::printf("00743470_TEST FAIL\n"); return 1; }
    std::printf("00743470_TEST PASS\n");
    return 0;
}
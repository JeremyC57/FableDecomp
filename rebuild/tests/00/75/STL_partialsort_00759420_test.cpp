// Behaviour test for 0x00759420 _STL::__partial_sort over the 0x2C-byte wound-morph entries
#include <cstdio>

struct CWoundMorphsEntry { long Words[10]; };
template <class A, class B> struct pair {
    A first;
    B second;
    pair() {}
    pair(const pair& other) : first(other.first), second(other.second) {}
};
typedef pair<int, CWoundMorphsEntry> TWoundPair;
struct CKeyPairCompareLess {
    bool operator()(const TWoundPair& a, const TWoundPair& b) const { return a.first < b.first; }
};

void __push_heap(TWoundPair* first, int holeIndex, int topIndex, TWoundPair val, CKeyPairCompareLess comp)
{
    int parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), val)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = val;
}
void __adjust_heap(TWoundPair* first, int holeIndex, int len, TWoundPair val, CKeyPairCompareLess comp)
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
void __make_heap(TWoundPair* first, TWoundPair* last, CKeyPairCompareLess comp, TWoundPair*, int*)
{
    if (last - first < 2)
        return;
    int len = last - first;
    int parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, TWoundPair(*(first + parent)), comp);
        if (parent == 0)
            return;
        parent--;
    }
}
void sort_heap(TWoundPair* first, TWoundPair* last, CKeyPairCompareLess comp)
{
    while (last - first > 1) {
        --last;
        TWoundPair val(*last);
        *last = *first;
        __adjust_heap(first, 0, int(last - first), val, comp);
    }
}
inline void make_heap(TWoundPair* first, TWoundPair* last, CKeyPairCompareLess comp)
{
    __make_heap(first, last, comp, (TWoundPair*)0, (int*)0);
}
template <class RandomAccessIter, class T, class Compare, class Distance>
inline void __pop_heap(RandomAccessIter first, RandomAccessIter last, RandomAccessIter result, T val, Compare comp, Distance*)
{
    *result = *first;
    __adjust_heap(first, Distance(0), Distance(last - first), val, comp);
}

void __partial_sort(TWoundPair* first, TWoundPair* middle, TWoundPair* last, TWoundPair*, CKeyPairCompareLess comp)
{
    make_heap(first, middle, comp);
    for (TWoundPair* i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, TWoundPair(*i), comp, (int*)0);
    sort_heap(first, middle, comp);
}

int main()
{
    int src[12] = { 31, -4, 17, 8, 8, 99, -20, 5, 0, 63, 2, 11 };
    int sorted[12] = { -20, -4, 0, 2, 5, 8, 8, 11, 17, 31, 63, 99 };
    int ok = 1;
    for (int k = 0; k <= 12; ++k) {
        TWoundPair a[12];
        for (int i = 0; i < 12; ++i) { a[i].first = src[i]; for (int w = 0; w < 10; ++w) a[i].second.Words[w] = src[i] * 3 + w; }
        __partial_sort(a, a + k, a + 12, (TWoundPair*)0, CKeyPairCompareLess());
        for (int i = 0; i < k; ++i) if (a[i].first != sorted[i]) ok = 0;
        int sum = 0;
        for (int i = 0; i < 12; ++i) { sum += a[i].first; for (int w = 0; w < 10; ++w) if (a[i].second.Words[w] != a[i].first * 3 + w) ok = 0; }
        if (sum != 220) ok = 0;
    }
    if (!ok) { std::printf("00759420_TEST FAIL\n"); return 1; }
    std::printf("00759420_TEST PASS\n");
    return 0;
}
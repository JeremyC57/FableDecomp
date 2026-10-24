// Behaviour test for 0x005F9D4E std::__partial_sort(first, middle, last, pair<long, EHeroAbility>*, CKeyPairCompareLess): make_heap on
// [first, middle), pop every smaller element of [middle, last) into the heap, sort_heap -- the smallest (middle - first)
// keys end up sorted at the front. The heap helpers (make_heap/__pop_heap/sort_heap, separate retail bodies) are modelled.
#include <cstdio>

enum EHeroAbility { ABILITY_NULL = 0, EHeroAbility_Force32 = 0x7fffffff };

namespace stl {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() : first(), second() { }
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair& o) : first(o.first), second(o.second) { }
};
}

template <class K, class V> struct CKeyPairCompareLess {
    bool operator()(const stl::pair<K, V>& a, const stl::pair<K, V>& b) const { return a.first < b.first; }
};

namespace stl {
template <class RandomAccessIter, class Distance, class T, class Compare>
void __push_heap(RandomAccessIter first, Distance holeIndex, Distance topIndex, T val, Compare comp)
{
    Distance parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), val)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = val;
}
template <class RandomAccessIter, class Distance, class T, class Compare>
void __adjust_heap(RandomAccessIter first, Distance holeIndex, Distance len, T val, Compare comp)
{
    Distance topIndex = holeIndex;
    Distance secondChild = 2 * holeIndex + 2;
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
template <class RandomAccessIter, class T, class Compare, class Distance>
void __pop_heap(RandomAccessIter first, RandomAccessIter last, RandomAccessIter result, T value, Compare comp, Distance*)
{
    *result = *first;
    __adjust_heap(first, Distance(0), Distance(last - first), value, comp);
}
template <class RandomAccessIter, class Compare>
void make_heap(RandomAccessIter first, RandomAccessIter last, Compare comp)
{
    int len = int(last - first);
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
template <class RandomAccessIter, class Compare>
void sort_heap(RandomAccessIter first, RandomAccessIter last, Compare comp)
{
    while (last - first > 1) {
        --last;
        __pop_heap(first, last, last, *last, comp, (int*)0);
    }
}
}

namespace stl {
template <class RandomAccessIter, class T, class Compare>
void __partial_sort(RandomAccessIter first, RandomAccessIter middle, RandomAccessIter last, T*, Compare comp)
{
    make_heap(first, middle, comp);
    for (RandomAccessIter i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, T(*i), comp, (int*)0);
    sort_heap(first, middle, comp);
}
}

typedef stl::pair<long, EHeroAbility> P;

int main()
{
    int ok = 1;
    long keys[10] = { 42, 7, 19, 3, 88, 25, 1, 64, 12, 30 };
    P v[10];
    for (int i = 0; i < 10; ++i)
        v[i] = P(keys[i], (EHeroAbility)(1000 + keys[i]));
    stl::__partial_sort(v, v + 4, v + 10, (P*)0, CKeyPairCompareLess<long, EHeroAbility>());
    long want[4] = { 1, 3, 7, 12 };
    for (int i = 0; i < 4; ++i)
        if (v[i].first != want[i] || v[i].second != (EHeroAbility)(1000 + want[i])) ok = 0;
    long sum = 0;
    for (int i = 0; i < 10; ++i) {
        sum += v[i].first;
        if (v[i].second != (EHeroAbility)(1000 + v[i].first)) ok = 0;
    }
    if (sum != 291) ok = 0;
    // middle == last: a full heap sort
    P w[5];
    long k2[5] = { 5, 4, 3, 2, 1 };
    for (int i = 0; i < 5; ++i) w[i] = P(k2[i], (EHeroAbility)i);
    stl::__partial_sort(w, w + 5, w + 5, (P*)0, CKeyPairCompareLess<long, EHeroAbility>());
    for (int i = 0; i < 5; ++i)
        if (w[i].first != i + 1) ok = 0;
    if (!ok) { std::printf("005f9d4e_TEST FAIL\n"); return 1; }
    std::printf("005f9d4e_TEST PASS\n");
    return 0;
}
// Behaviour test for 0x005F86AA std::__adjust_heap(pair<long, long>* first, int hole, int len, pair<long, long> val, CKeyPairCompareLess):
// sift the hole down to a leaf, then __push_heap (0x005F7D85 / 0x005F7DD4, modelled here) the value back up.
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
}

typedef stl::pair<long, long> P;

static int IsHeap(const P* v, int n)
{
    for (int i = 1; i < n; ++i)
        if (v[(i - 1) / 2].first < v[i].first)
            return 0;
    return 1;
}

int main()
{
    int ok = 1;
    CKeyPairCompareLess<long, long> cmp;
    // pop-heap step: a max-heap of 7 whose root is replaced by a small value
    long keys[7] = { 90, 80, 70, 40, 50, 60, 10 };
    P v[7];
    for (int i = 0; i < 7; ++i)
        v[i] = P(keys[i], (long)(i + 1));
    stl::__adjust_heap(v, 0, 7, P(5, (long)42), cmp);
    if (!IsHeap(v, 7) || v[0].first != 80) ok = 0;
    long sum = 0;
    int found = 0;
    for (int i = 0; i < 7; ++i) {
        sum += v[i].first;
        if (v[i].first == 5 && v[i].second == (long)42) found = 1;
    }
    if (sum != 80 + 70 + 40 + 50 + 60 + 10 + 5 || !found) ok = 0;
    // even length: the lone left child at secondChild == len is taken
    P w[4];
    w[0] = P(50, (long)1); w[1] = P(40, (long)2); w[2] = P(30, (long)3); w[3] = P(20, (long)4);
    stl::__adjust_heap(w, 1, 4, P(1, (long)9), cmp);
    if (w[1].first != 20 || w[3].first != 1 || w[3].second != (long)9 || !IsHeap(w, 4)) ok = 0;
    if (!ok) { std::printf("005f86aa_TEST FAIL\n"); return 1; }
    std::printf("005f86aa_TEST PASS\n");
    return 0;
}
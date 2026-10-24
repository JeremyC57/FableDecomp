// Behaviour test for 0x005F9A98 std::__linear_insert(pair<long, EHeroAbility>* first, pair<long, EHeroAbility>* last, pair<long, EHeroAbility> val, CKeyPairCompareLess):
// a value smaller than *first shifts [first, last) up one (copy_backward) and lands at first, else the guarded walk
// (__unguarded_linear_insert, modelled) places it.
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
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class RandomAccessIter, class BidirectionalIter, class Distance>
BidirectionalIter __copy_backward(RandomAccessIter first, RandomAccessIter last, BidirectionalIter result,
                                  const random_access_iterator_tag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}
template <class RandomAccessIter, class T, class Compare>
void __unguarded_linear_insert(RandomAccessIter last, T val, Compare comp)
{
    RandomAccessIter next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}
template <class BidirectionalIter1, class BidirectionalIter2>
inline BidirectionalIter2 copy_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}
template <class RandomAccessIter, class T, class Compare>
void __linear_insert(RandomAccessIter first, RandomAccessIter last, T val, Compare comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}
}

typedef stl::pair<long, EHeroAbility> P;

int main()
{
    int ok = 1;
    CKeyPairCompareLess<long, EHeroAbility> cmp;
    P v[4];
    v[0] = P(10, (EHeroAbility)1); v[1] = P(20, (EHeroAbility)2); v[2] = P(30, (EHeroAbility)3); v[3] = P(5, (EHeroAbility)4);
    stl::__linear_insert(v, v + 3, P(v[3]), cmp);          // new minimum: front insert
    if (v[0].first != 5 || v[0].second != (EHeroAbility)4 || v[1].first != 10 || v[3].first != 30 || v[3].second != (EHeroAbility)3) ok = 0;
    P w[4];
    w[0] = P(10, (EHeroAbility)1); w[1] = P(20, (EHeroAbility)2); w[2] = P(30, (EHeroAbility)3); w[3] = P(25, (EHeroAbility)4);
    stl::__linear_insert(w, w + 3, P(w[3]), cmp);          // interior: guarded walk
    if (w[0].first != 10 || w[1].first != 20 || w[2].first != 25 || w[2].second != (EHeroAbility)4 || w[3].first != 30) ok = 0;
    if (!ok) { std::printf("005f9a98_TEST FAIL\n"); return 1; }
    std::printf("005f9a98_TEST PASS\n");
    return 0;
}
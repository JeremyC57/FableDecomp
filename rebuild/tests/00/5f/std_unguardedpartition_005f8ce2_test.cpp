// Behaviour test for 0x005F8CE2 std::__unguarded_partition(pair<long, long>* first, pair<long, long>* last, pair<long, long> pivot, CKeyPairCompareLess).
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

template <class ForwardIter1, class ForwardIter2, class T>
inline void __iter_swap_aux(ForwardIter1 a, ForwardIter2 b, T*)
{
    T tmp = *a;
    *a = *b;
    *b = tmp;
}

template <class ForwardIter1, class ForwardIter2>
inline void iter_swap(ForwardIter1 a, ForwardIter2 b)
{
    __iter_swap_aux(a, b, a);
}
}

template <class K, class V> struct CKeyPairCompareLess {
    bool operator()(const stl::pair<K, V>& a, const stl::pair<K, V>& b) const { return a.first < b.first; }
};

namespace stl {
template <class RandomAccessIter, class T, class Compare>
RandomAccessIter __unguarded_partition(RandomAccessIter first, RandomAccessIter last, T pivot, Compare comp)
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
}

typedef stl::pair<long, long> P;

int main()
{
    long keys[8] = { 7, 2, 9, 4, 5, 1, 8, 3 };
    P v[8];
    for (int i = 0; i < 8; ++i)
        v[i] = P(keys[i], (long)(100 + keys[i]));
    P* cut = stl::__unguarded_partition(v, v + 8, P(5, (long)0), CKeyPairCompareLess<long, long>());
    int ok = cut > v && cut < v + 8;
    for (P* p = v; p < cut; ++p)
        if (p->first > 5) ok = 0;
    for (P* p = cut; p < v + 8; ++p)
        if (p->first < 5) ok = 0;
    long sum = 0;
    for (int i = 0; i < 8; ++i) {
        sum += v[i].first;
        if (v[i].second != (long)(100 + v[i].first)) ok = 0;   // pairs move as a unit
    }
    if (sum != 39) ok = 0;
    if (!ok) { std::printf("005f8ce2_TEST FAIL\n"); return 1; }
    std::printf("005f8ce2_TEST PASS\n");
    return 0;
}
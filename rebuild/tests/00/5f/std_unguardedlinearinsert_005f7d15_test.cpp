// Behaviour test for 0x005F7D15 std::__unguarded_linear_insert(pair<long, long>* last, pair<long, long> val, CKeyPairCompareLess).
#include <cstdio>

enum EHeroAbility { ABILITY_NULL = 0, ABILITY_A = 3, ABILITY_B = 7, ABILITY_C = 11, EHeroAbility_Force32 = 0x7fffffff };

namespace stl {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() : first(), second() { }
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair& o) : first(o.first), second(o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess { public:
    bool operator()(const stl::pair<K, V>& a, const stl::pair<K, V>& b) const { return a.first < b.first; }
};

namespace stl {
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
}

typedef stl::pair<long, long> P;

int main()
{
    // a sorted prefix with a smaller guard at [0]; insert the element sitting at [4]
    P v[5];
    v[0] = P(-100, (long)ABILITY_NULL);
    v[1] = P(10, (long)ABILITY_A);
    v[2] = P(20, (long)ABILITY_B);
    v[3] = P(30, (long)ABILITY_C);
    v[4] = P(15, (long)ABILITY_B);
    stl::__unguarded_linear_insert(v + 4, P(v[4]), CKeyPairCompareLess<long, long>());
    int ok = v[0].first == -100 && v[1].first == 10 && v[2].first == 15 && v[2].second == (long)ABILITY_B &&
             v[3].first == 20 && v[3].second == (long)ABILITY_B && v[4].first == 30 && v[4].second == (long)ABILITY_C;
    // equal keys stop the walk (strict less): the new element lands after its equal
    P w[3];
    w[0] = P(5, (long)ABILITY_A);
    w[1] = P(7, (long)ABILITY_B);
    w[2] = P(7, (long)ABILITY_C);
    stl::__unguarded_linear_insert(w + 2, P(w[2]), CKeyPairCompareLess<long, long>());
    if (w[1].second != (long)ABILITY_B || w[2].second != (long)ABILITY_C) ok = 0;
    if (!ok) { std::printf("005f7d15_TEST FAIL\n"); return 1; }
    std::printf("005f7d15_TEST PASS\n");
    return 0;
}
// Behaviour test for 0x005F84C6 std::__median(const pair<long, EHeroAbility>&, const pair<long, EHeroAbility>&, const pair<long, EHeroAbility>&, CKeyPairCompareLess).
#include <cstdio>

enum EHeroAbility { ABILITY_NULL = 0, ABILITY_A = 3, ABILITY_B = 7, ABILITY_C = 11, EHeroAbility_Force32 = 0x7fffffff };

namespace stl {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair& o) : first(o.first), second(o.second) { }
};
}

template <class K, class V> struct CKeyPairCompareLess {
    bool operator()(const stl::pair<K, V>& a, const stl::pair<K, V>& b) const { return a.first < b.first; }
};

namespace stl {
template <class T, class Compare>
const T& __median(const T& a, const T& b, const T& c, Compare comp)
{
    if (comp(a, b))
        if (comp(b, c))
            return b;
        else if (comp(a, c))
            return c;
        else
            return a;
    else if (comp(a, c))
        return a;
    else if (comp(b, c))
        return c;
    else
        return b;
}
}

typedef stl::pair<long, EHeroAbility> P;

int main()
{
    P lo(1, (EHeroAbility)ABILITY_A), mid(5, (EHeroAbility)ABILITY_B), hi(9, (EHeroAbility)ABILITY_C);
    CKeyPairCompareLess<long, EHeroAbility> cmp;
    int ok = 1;
    // every permutation returns the middle key, by reference to the argument holding it
    if (&stl::__median(lo, mid, hi, cmp) != &mid) ok = 0;
    if (&stl::__median(lo, hi, mid, cmp) != &mid) ok = 0;
    if (&stl::__median(mid, lo, hi, cmp) != &mid) ok = 0;
    if (&stl::__median(mid, hi, lo, cmp) != &mid) ok = 0;
    if (&stl::__median(hi, lo, mid, cmp) != &mid) ok = 0;
    if (&stl::__median(hi, mid, lo, cmp) != &mid) ok = 0;
    // ties: (a == b) falls to the comp(a, c) / comp(b, c) arms
    P twin(5, (EHeroAbility)ABILITY_A);
    if (&stl::__median(mid, twin, hi, cmp) != &mid) ok = 0;
    if (&stl::__median(mid, twin, lo, cmp) != &twin) ok = 0;
    if (!ok) { std::printf("005f84c6_TEST FAIL\n"); return 1; }
    std::printf("005f84c6_TEST PASS\n");
    return 0;
}
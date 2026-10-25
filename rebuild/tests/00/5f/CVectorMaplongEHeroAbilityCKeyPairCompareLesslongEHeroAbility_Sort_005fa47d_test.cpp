// Behaviour test for CVectorMap<long, EHeroAbility, CKeyPairCompareLess<long, EHeroAbility> >::Sort (0x005fa47d): sorts the pairs by key, clears Dirty.
#include <stdio.h>
namespace std {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class T> void swap_stand_in(T& a, T& b) { T t = a; a = b; b = t; }
template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)   // insertion-sort stand-in
{
    for (_RandomAccessIter i = __first; i != __last; ++i)
        for (_RandomAccessIter j = i; j != __first && __comp(*j, *(j - 1)); --j)
            swap_stand_in(*j, *(j - 1));
}
}
enum EHeroAbility { HERO_ABILITY_NULL };
template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
template <class K, class V, class C> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    value_type* First; value_type* Last; value_type* EndOfStorage;
    C Compare; bool Dirty;
    void Sort() { std::sort(First, Last, Compare); Dirty = false; }
};
int main()
{
    std::pair<long, EHeroAbility> e[4];
    long keys[4] = { 7, 2, 9, 1 };
    for (int i = 0; i < 4; ++i) { e[i].first = keys[i]; *(long*)&e[i].second = keys[i] * 10; }
    CVectorMap<long, EHeroAbility, CKeyPairCompareLess<long, EHeroAbility> > m;
    m.First = e; m.Last = e + 4; m.EndOfStorage = e + 4; m.Dirty = true;
    m.Sort();
    int ok = !m.Dirty;
    for (int i = 0; i < 4; ++i) ok = ok && (long)e[i].second == e[i].first * 10 && (i == 0 || e[i - 1].first <= e[i].first);
    printf(ok ? "005fa47d_TEST PASS\n" : "005fa47d_TEST FAIL\n");
    return ok ? 0 : 1;
}

// Behaviour test for std::__unguarded_insertion_sort_aux(pair<pair<CCharString, CCharString>, CSoundBankLH*>*, ..., CKeyPairCompareLess) (0x00A40FF0): after the pass [first, last) is in ascending key order (the entry
// before first is a sentinel no greater than any key), equal keys keep their order and payloads travel with keys.
#include <cstdio>
#include <cstring>
#include <cstdlib>

class CCharString {
public:
    const char* Text;
    CCharString() : Text("") { }
    CCharString(const char* t) : Text(t) { }
    CCharString(const CCharString& o) : Text(o.Text) { }
    CCharString& operator=(const CCharString& o) { Text = o.Text; return *this; }
    bool operator<(const CCharString& o) const { return std::strcmp(Text, o.Text) < 0; }
};
class CSoundBankLH { public: int Id; };
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair& __p) : first(__p.first), second(__p.second) { }
};
template <class _T1, class _T2>
inline bool operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return __x.first < __y.first || (!(__y.first < __x.first) && __x.second < __y.second);
}
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _RandomAccessIter, class _Tp, class _Compare>
void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}
template <class _RandomAccessIter, class _Tp, class _Compare>
void __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<CCharString, CCharString> SNamePair;
typedef std::pair<SNamePair, CSoundBankLH*> E;

int main()
{
    static CSoundBankLH banks[6];
    static const char* k1[6] = { "music", "ambient", "music", "ambient", "voice", "ambient" };
    static const char* k2[6] = { "town", "forest", "cave", "forest", "hero", "cave" };
    E a[7];
    a[0] = E(SNamePair(CCharString(""), CCharString("")), (CSoundBankLH*)0);
    for (int i = 0; i < 6; ++i)
        a[i + 1] = E(SNamePair(CCharString(k1[i]), CCharString(k2[i])), &banks[i]);
    std::__unguarded_insertion_sort_aux(a + 1, a + 7, (E*)0, CKeyPairCompareLess<SNamePair, CSoundBankLH*>());
    static const int order[6] = { 5, 1, 3, 2, 0, 4 };   // (ambient,cave) (ambient,forest)x2 (music,cave) (music,town) (voice,hero)
    int fails = 0;
    for (int i = 0; i < 6; ++i) {
        if (std::strcmp(a[i + 1].first.first.Text, k1[order[i]]) != 0) ++fails;
        if (std::strcmp(a[i + 1].first.second.Text, k2[order[i]]) != 0) ++fails;
        if (a[i + 1].second != &banks[order[i]]) ++fails;
    }
    if (fails) { std::printf("00a40ff0_TEST FAIL\n"); return 1; }
    std::printf("00a40ff0_TEST PASS\n");
    return 0;
}

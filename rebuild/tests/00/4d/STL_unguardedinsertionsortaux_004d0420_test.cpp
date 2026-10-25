// Behaviour test for _STL::__unguarded_insertion_sort_aux(pair<float, CCharString>*, ..., CKeyPairCompareLess<float, CCharString>) (0x004D0420): after the pass [first, last) is in ascending key order (the entry
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
namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair& __p) : first(__p.first), second(__p.second) { }
};
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
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

typedef _STL::pair<float, CCharString> E;

int main()
{
    static char p0[] = "m0", p1[] = "m1", p2[] = "m2", p3[] = "m3", p4[] = "m4", p5[] = "m5";
    static const char* payload[6] = { p0, p1, p2, p3, p4, p5 };
    static const float keys[6] = { 4.0f, 1.0f, 5.0f, 2.0f, 1.0f, 3.0f };
    E a[7];
    a[0] = E(-1.0f, CCharString(""));
    for (int i = 0; i < 6; ++i)
        a[i + 1] = E(keys[i], CCharString(payload[i]));
    _STL::__unguarded_insertion_sort_aux(a + 1, a + 7, (E*)0, CKeyPairCompareLess<float, CCharString>());
    static const int order[6] = { 1, 4, 3, 5, 0, 2 };
    int fails = 0;
    for (int i = 0; i < 6; ++i) {
        float d = a[i + 1].first - keys[order[i]];
        if (d > 1e-6f || d < -1e-6f) ++fails;
        if (a[i + 1].second.Text != payload[order[i]]) ++fails;
    }
    if (fails) { std::printf("004d0420_TEST FAIL\n"); return 1; }
    std::printf("004d0420_TEST PASS\n");
    return 0;
}

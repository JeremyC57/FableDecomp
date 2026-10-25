// Behaviour test for _STL::__unguarded_insertion_sort_aux(pair<CCharString, C3DMeshVolume>*, ..., CKeyPairCompareLess<CCharString, C3DMeshVolume>) (0x00A710F0): after the pass [first, last) is in ascending key order (the
// entry before first is a sentinel no greater than any key), equal keys keep their order and payloads travel with keys.
#include <cstdio>
#include <cstring>

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
static int gRefs[8];
class CCountedPointer {                // the volume's counted shape pointer: copies add a reference
public:
    int Index;
    CCountedPointer() : Index(7) { ++gRefs[7]; }
    explicit CCountedPointer(int i) : Index(i) { ++gRefs[i]; }
    CCountedPointer(const CCountedPointer& o) : Index(o.Index) { ++gRefs[Index]; }
    ~CCountedPointer() { --gRefs[Index]; }
    CCountedPointer& operator=(const CCountedPointer& o) { ++gRefs[o.Index]; --gRefs[Index]; Index = o.Index; return *this; }
};
class C3DMeshVolume {
public:
    long            Type;
    float           Size[3];
    CCountedPointer Shape;
    C3DMeshVolume() : Type(0) { Size[0] = Size[1] = Size[2] = 0.0f; }
    explicit C3DMeshVolume(int i) : Type(i), Shape(i) { Size[0] = Size[1] = Size[2] = (float)i; }
};

typedef _STL::pair<CCharString, C3DMeshVolume> E;

int main()
{
    static const char* names[6] = { "delta", "alpha", "echo", "bravo", "alpha", "charlie" };
    E a[7];
    a[0] = E(CCharString(""), C3DMeshVolume());
    for (int i = 0; i < 6; ++i)
        a[i + 1] = E(CCharString(names[i]), C3DMeshVolume(i));
    _STL::__unguarded_insertion_sort_aux(a + 1, a + 7, (E*)0, CKeyPairCompareLess<CCharString, C3DMeshVolume>());
    static const char* sorted[6] = { "alpha", "alpha", "bravo", "charlie", "delta", "echo" };
    static const int order[6] = { 1, 4, 3, 5, 0, 2 };
    int fails = 0;
    for (int i = 0; i < 6; ++i) {
        if (std::strcmp(a[i + 1].first.Text, sorted[i]) != 0) ++fails;
        if ((int)a[i + 1].second.Type + (a[i + 1].second.Shape.Index != a[i + 1].second.Type ? 100 : 0) != order[i]) ++fails;
    }
    if (fails) { std::printf("00a710f0_TEST FAIL\n"); return 1; }
    std::printf("00a710f0_TEST PASS\n");
    return 0;
}
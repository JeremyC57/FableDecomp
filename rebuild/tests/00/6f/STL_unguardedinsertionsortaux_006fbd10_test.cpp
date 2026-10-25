// Behaviour test for _STL::__unguarded_insertion_sort_aux(pair<unsigned long, CSmallVector<CEvent,32> >*, ..., CKeyPairCompareLess) (0x006FBD10): after the pass [first, last) is in ascending key order (the entry
// before first is a sentinel no greater than any key), equal keys keep their order and payloads travel with keys.
#include <cstdio>
#include <cstring>
#include <cstdlib>

namespace NAnimationEvents {
class CEvent {
public:
    unsigned long Index : 30;
    unsigned long ExternallyImported : 1;
    unsigned long StopEvent : 1;
    float Time;
};
}

static int gLive = 0;

template <class T, int N> class CSmallVector {            // deep-copying small vector (the Copy 0x006FB5C0 role)
public:
    T*  Start;
    int Size;
    CSmallVector() : Start(0), Size(0) { }
    CSmallVector(const CSmallVector& other) : Start(0), Size(0) { Copy(other); }
    ~CSmallVector() { if (Start) { std::free(Start); --gLive; } }
    CSmallVector& operator=(const CSmallVector& other)
    {
        if (this != &other) {
            if (Start) { std::free(Start); --gLive; }
            Start = 0; Size = 0;
            Copy(other);
        }
        return *this;
    }
    void Copy(const CSmallVector& other)
    {
        if (other.Size) {
            Start = (T*)std::malloc(other.Size * sizeof(T));
            ++gLive;
            for (int i = 0; i < other.Size; ++i)
                Start[i] = other.Start[i];
        }
        Size = other.Size;
    }
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

typedef CSmallVector<NAnimationEvents::CEvent, 32> TEventList;
typedef _STL::pair<unsigned long, TEventList> E;

static TEventList MakeList(int tag)
{
    TEventList base;
    NAnimationEvents::CEvent ev[2];
    ev[0].Index = (unsigned long)tag; ev[0].ExternallyImported = 0; ev[0].StopEvent = 1; ev[0].Time = 0.5f * tag;
    ev[1] = ev[0]; ev[1].StopEvent = 0;
    TEventList src;
    src.Start = ev; src.Size = 2;
    base.Copy(src);
    src.Start = 0; src.Size = 0;
    return base;
}

int main()
{
    static const unsigned long keys[6] = { 40u, 10u, 50u, 20u, 10u, 30u };
    int fails = 0;
    {
        E a[7] = { E(0u, TEventList()), E(keys[0], MakeList(0)), E(keys[1], MakeList(1)), E(keys[2], MakeList(2)),
                   E(keys[3], MakeList(3)), E(keys[4], MakeList(4)), E(keys[5], MakeList(5)) };
        _STL::__unguarded_insertion_sort_aux(a + 1, a + 7, (E*)0, CKeyPairCompareLess<unsigned long, TEventList>());
        static const int order[6] = { 1, 4, 3, 5, 0, 2 };
        for (int i = 0; i < 6; ++i) {
            if (a[i + 1].first != keys[order[i]]) ++fails;
            if (a[i + 1].second.Size != 2 || a[i + 1].second.Start[0].Index != (unsigned long)order[i]) ++fails;
            float d = a[i + 1].second.Start[1].Time - 0.5f * order[i];
            if (d > 1e-6f || d < -1e-6f || a[i + 1].second.Start[0].StopEvent != 1) ++fails;
        }
        if (sizeof(E) != 12u && sizeof(void*) == 4u) ++fails;
    }
    if (gLive != 0) ++fails;                                 // every copy made by the pass was released
    if (fails) { std::printf("006fbd10_TEST FAIL\n"); return 1; }
    std::printf("006fbd10_TEST PASS\n");
    return 0;
}

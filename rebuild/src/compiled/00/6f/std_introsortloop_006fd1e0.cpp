// std::__introsort_loop(SKeyedEvents* first, SKeyedEvents* last, SKeyedEvents*, int depth_limit, CKeyPairCompareLess comp) -- STLport introsort recursion over the keyed anim-event vector: median-of-three partition until 16 elements remain, partial (heap) sort when the depth budget runs out
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FD1E0, 254 bytes, __fastcall (first ecx, last edx), `ret 0Ch` (the _Tp* tag,
//   the depth limit, the comparator).
// Label correction: catalog "std_sort_Heapify_Recursive" -> STLport 4.x __introsort_loop over
//   pair<unsigned long, CSmallVector<NAnimationEvents::CEvent,32> > (called by the landed sort 0x006FD380 with
//   `__lg(n) * 2`, and recursively).
// Callees: CSmallVector::Copy 0x006FB5C0 (the pivot's in-place pair copy), __unguarded_partition 0x006FBEE0 (pivot
//   by value), __partial_sort 0x006FCEA0, itself.
// Shape: `while (last - first > __stl_threshold)` (12-byte divide, `cmp eax,10h`); `if (depth_limit == 0)
//   { partial_sort(first, last, last, comp); return; }`; `--depth_limit` in its own argument slot; __median inlined
//   with the comparator's key compares (`jae`/`jb`: unsigned keys); the pivot pair is copy-constructed straight into
//   the by-value argument area (key, zeroed CSmallVector, Copy); `last = cut`.
void __cdecl operator delete[](void* block);   // MSVCR71 import thunk 0x00BFEB1C

namespace NAnimationEvents { class CEvent { public: long m_a, m_b; }; }

template<class T, int N> class CSmallVector {
public:
    T* Start;                                   // +0x00
    int Size;                                   // +0x04
    void Copy(const CSmallVector& other);       // 0x006FB5C0
    CSmallVector(const CSmallVector& other) : Start(0), Size(0) { Copy(other); }
    ~CSmallVector() { delete[] Start; Start = 0; Size = 0; }
};

namespace std {

template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& other) : first(other.first), second(other.second) {}
};

}

typedef std::pair<unsigned long, CSmallVector<NAnimationEvents::CEvent, 32> > SKeyedEvents;

template<class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<unsigned long, CSmallVector<NAnimationEvents::CEvent, 32> > SKeyedEventsLess;

namespace std {

const int __stl_threshold = 16;

template <class _Tp, class _Compare>
inline const _Tp& __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
    if (__comp(__a, __b))
        if (__comp(__b, __c))
            return __b;
        else if (__comp(__a, __c))
            return __c;
        else
            return __a;
    else if (__comp(__a, __c))
        return __a;
    else if (__comp(__b, __c))
        return __c;
    else
        return __b;
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp);   // 0x006FBEE0

template <class _RandomAccessIterator, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIterator __first, _RandomAccessIterator __middle, _RandomAccessIterator __last, _Tp*, _Compare __comp);   // 0x006FCEA0

template <class _RandomAccessIterator, class _Compare>
inline void partial_sort(_RandomAccessIterator __first, _RandomAccessIterator __middle, _RandomAccessIterator __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, (SKeyedEvents*)0, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp)
{
    while (__last - __first > __stl_threshold) {
        if (__depth_limit == 0) {
            partial_sort(__first, __last, __last, __comp);
            return;
        }
        --__depth_limit;
        _RandomAccessIter __cut =
            __unguarded_partition(__first, __last,
                                  _Tp(__median(*__first, *(__first + (__last - __first) / 2), *(__last - 1), __comp)),
                                  __comp);
        __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp);
        __last = __cut;
    }
}

template void __fastcall __introsort_loop<SKeyedEvents*, SKeyedEvents, int, SKeyedEventsLess>(SKeyedEvents*, SKeyedEvents*, SKeyedEvents*, int, SKeyedEventsLess);

}
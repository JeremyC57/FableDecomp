// std::__introsort_loop(pair<unsigned long, CCountedPointer<CFaction> >* first, pair* last, pair*, int depth_limit, CKeyPairCompareLess comp) -- STLport introsort loop of the faction vector map: median-of-three partition and recurse on the right part while more than 16 entries remain, falling back to partial_sort when the depth limit runs out
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BB040, 248 bytes (+ int3 padding to 256), `ret 0Ch`.
// Identity: the catalog's "Heap_PopRecursive" is a descriptive auto label. The landed sort 0x006BB140 calls this body as
//   `__introsort_loop(first, last, (T*)0, __lg(n) * 2, comp)` and it recurses into itself (0x006BB0F9). Element
//   pair<unsigned long, CCountedPointer<CFaction> > (12 bytes: key +0, Data +4, Info +8) and the empty key-only
//   comparator CKeyPairCompareLess<unsigned long, CCountedPointer<CFaction> >, as the landed partition 0x006BA100.
// Callees: __unguarded_partition(first, last, T pivot, comp) 0x006BA100, __partial_sort(first, middle, last, T*, comp)
//   0x006BADB0 (partial_sort's out-of-line worker), itself.
// Shape: STLport 4.6 _algo.c `__introsort_loop` verbatim from a /Gr TU (`__fastcall`: first in ecx -> edi, last in edx ->
//   ebx; the value-type pointer, the depth limit and the 4-byte comparator on the stack). `__median` is inlined (unsigned
//   key compares choosing a reference), the pivot `_Tp(__median(..))` is built in the argument area by STLport pair's
//   memberwise copy constructor (key, Data, Info and the null-tested reference-count increment) after the comparator push;
//   partial_sort is the inline front end that passes `(_Tp*)0`.
void operator delete(void* p);                              // 0x00BFE9BC (the release, not emitted here)

class CFaction;

class CCPPointerInfo {                                      // 12 bytes (Ego_r.pdb)
public:
    unsigned long RefCount;                                 // +0x00
    void (__fastcall* DeleteFunc)(void*);                   // +0x04
    void*         Data;                                     // +0x08
};

template <class T>
class CCountedPointer {                                     // 8 bytes
public:
    T*              Data;                                   // +0x00
    CCPPointerInfo* Info;                                   // +0x04
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer()
    {
        if (Info && --Info->RefCount == 0) {
            Info->DeleteFunc(Info->Data);
            delete Info;
        }
    }
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V>
class CKeyPairCompareLess {                                 // empty comparator, passed by value
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

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
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot,
                                                   _Compare __comp);                         // 0x006BA100
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp);                                             // 0x006BADB0

template <class _Tp, class _Compare>
inline void partial_sort(_Tp* __first, _Tp* __middle, _Tp* __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, (_Tp*)0, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit,
                                 _Compare __comp)
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

typedef pair<unsigned long, CCountedPointer<CFaction> > _FactionEntry;
template void __fastcall __introsort_loop<_FactionEntry*, _FactionEntry, int, CKeyPairCompareLess<unsigned long, CCountedPointer<CFaction> > >(
    _FactionEntry*, _FactionEntry*, _FactionEntry*, int, CKeyPairCompareLess<unsigned long, CCountedPointer<CFaction> >);

} // namespace std
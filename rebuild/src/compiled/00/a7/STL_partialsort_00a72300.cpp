// std::__partial_sort(pair<CCharString,C3DHelperPoint>* first, pair<CCharString,C3DHelperPoint>* middle, pair<CCharString,C3DHelperPoint>* last, pair<CCharString,C3DHelperPoint>*, CCompareString<C3DHelperPoint> comp) -- heap-select the smallest (middle - first) helper-point entries (name order) into [first, middle) and sort them
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A72300, 126 bytes (+2 int3), /Gr `__fastcall` (first in ecx, middle
//   in edx; last, the value-type tag pointer and the comparator on the stack, `ret 0Ch`).
// Callees: std::__make_heap 0x00A70630, CCharString::FastCompare 0x009E6530 (the CCompareString comparator),
//   CCharString::CCharString(const CCharString&) 0x0099EC30 (the by-value copy for __pop_heap), std::__pop_heap
//   0x00A701D0, std::sort_heap 0x00A719A0.
// Identity: the manifest's "QuickPartition_Generic" is descriptive. 20-byte pair<CCharString, C3DHelperPoint> entries
//   of CPhysicsMesh::HelperPoints ordered by CCompareString (the introsort's depth-limit fallback).
// Shape: STLport `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, _Tp(*i), comp, __DISTANCE_TYPE(first)); sort_heap(first, middle, comp);` -- the
//   inline make_heap reaches __make_heap with the two null tag pointers; the by-value element is copied into
//   __pop_heap's argument area (string copy call, then the C3DHelperPoint words).
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    bool FastCompare(const CCharString& other) const;   // 0x009E6530
};

class C3DVector {                                       // user memberwise copy ctor
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

class C3DHelperPoint {                                  // 16 bytes (Ego_r.pdb)
public:
    C3DVector Position;                                 // +0x00
    long      BoneIndex;                                // +0x0C
};

namespace std {
template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class V> class CCompareString {
public:
    bool operator()(const std::pair<CCharString, V>& a, const std::pair<CCharString, V>& b) const { return a.first.FastCompare(b.first); }
};

namespace std {

template <class _Tp>
inline int* __distance_type(const _Tp*) { return (int*)0; }
template <class _Tp>
inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*);   // 0x00A70630

template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, __value_type(__first), __distance_type(__first));
}

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result, _Tp __val, _Compare __comp, _Distance*);   // 0x00A701D0

template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x00A719A0

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, __distance_type(__first));
    sort_heap(__first, __middle, __comp);
}

typedef pair<CCharString, C3DHelperPoint> THelperEntry;
template void __fastcall __partial_sort<THelperEntry*, THelperEntry, CCompareString<C3DHelperPoint> >(THelperEntry*, THelperEntry*, THelperEntry*, THelperEntry*, CCompareString<C3DHelperPoint>);

}
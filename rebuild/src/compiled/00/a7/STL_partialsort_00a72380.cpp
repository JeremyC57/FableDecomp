// _STL::__partial_sort(pair<CCharString,C3DDummyObject>* first, pair<CCharString,C3DDummyObject>* middle, pair<CCharString,C3DDummyObject>* last, pair<CCharString,C3DDummyObject>*, CCompareString<C3DDummyObject> comp) -- heap-select the smallest (middle - first) dummy-object entries (name order) into [first, middle) and sort them
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A72380, 125 bytes (+3 int3), /Gr `__fastcall` (first in ecx, middle
//   in edx; last, the value-type tag pointer and the comparator on the stack, `ret 0Ch`).
// Callees: _STL::__make_heap 0x00A706B0, CCharString::FastCompare 0x009E6530 (the CCompareString comparator),
//   CCharString::CCharString(const CCharString&) 0x0099EC30 (the by-value copy for __pop_heap), _STL::__pop_heap
//   0x00A70280, _STL::sort_heap 0x00A71A40.
// Identity: the manifest's "Introsort_QuickPartition_BlendedEnv" is descriptive. 56-byte pair<CCharString,
//   C3DDummyObject> entries of CPhysicsMesh::DummyObjects ordered by CCompareString (the introsort's depth-limit
//   fallback).
// Shape: STLport `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, _Tp(*i), comp, __DISTANCE_TYPE(first)); sort_heap(first, middle, comp);` -- the
//   inline make_heap reaches __make_heap with the two null tag pointers; the by-value element is copied into
//   __pop_heap's argument area (string copy call, then C3DDummyObject's implicit 13-dword `rep movsd`).
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    bool FastCompare(const CCharString& other) const;   // 0x009E6530
};

class CMatrix3x4 {                                      // 48 bytes
public:
    float M[12];
};

class C3DDummyObject {                                  // 0x34 bytes (Ego_r.pdb)
public:
    CMatrix3x4 Transform;                               // +0x00
    long       BoneIndex;                               // +0x30
};

namespace _STL {
template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class V> class CCompareString {
public:
    bool operator()(const _STL::pair<CCharString, V>& a, const _STL::pair<CCharString, V>& b) const { return a.first.FastCompare(b.first); }
};

namespace _STL {

template <class _Tp>
inline int* __distance_type(const _Tp*) { return (int*)0; }
template <class _Tp>
inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*);   // 0x00A706B0

template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, __value_type(__first), __distance_type(__first));
}

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result, _Tp __val, _Compare __comp, _Distance*);   // 0x00A70280

template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x00A71A40

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, __distance_type(__first));
    sort_heap(__first, __middle, __comp);
}

typedef pair<CCharString, C3DDummyObject> TDummyEntry;
template void __fastcall __partial_sort<TDummyEntry*, TDummyEntry, CCompareString<C3DDummyObject> >(TDummyEntry*, TDummyEntry*, TDummyEntry*, TDummyEntry*, CCompareString<C3DDummyObject>);

}
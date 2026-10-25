// std::__partial_sort(pair<EExpressionType, CExpressionSet::CEntry>* first, pair<EExpressionType, CEntry>* middle, pair<EExpressionType, CEntry>* last, pair<EExpressionType, CEntry>*, CKeyPairCompareLess<EExpressionType, CEntry> comp) -- STLport partial_sort core over a creature expression set's vector map: heapify [first, middle), pop every smaller later entry into the heap, then sort the heap
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086FEB0, length 160 bytes, `ret 0Ch`.
// Callers: the map's __introsort_loop 0x008700A0 (landed; 0x00870186, the depth-limit fallback) and 0x0086FFAC.
// Callees: __make_heap(first, last, comp, T*, int*) @ 0x0086F930, __adjust_heap(first, holeIndex, len, T value, comp)
//   @ 0x0086F450 (landed), sort_heap(first, last, comp) @ 0x0086FB40; all __fastcall.
// Label: catalog "std::introsort_3way_partial_sort" is descriptive; STLport 4.x `__partial_sort` verbatim.
// Element: pair<EExpressionType, CExpressionSet::CEntry> (12 bytes, signed key), identified as in this chunk's 0x0086F090.
// Shape: as this chunk's 0x0086D160 (the 16-byte skeletal-morph twin) and 0x006810E0: /Gr algorithm TU, make_heap the
//   inline forwarder (two null tags pushed before the comparator), __pop_heap `__forceinline`; here the middle pointer
//   is spilled to a frame word and reloaded around the value build.
enum EExpressionType { EXPRESSION_TYPE_NONE = 0 };

class CExpressionSet {
public:
    class CEntry {                                          // Ego_r.pdb, 8 bytes of POD
    public:
        EExpressionType Type;                               // +0x00
        long            AnimIndex;                          // +0x04
    };
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x00
    _T2 second;                                             // +0x04
    pair(const pair& __o) : first(__o.first), second(__o.second) { }   // STLport's explicit pair copy constructor
};

template <class _RandomAccessIter, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp, _Tp*, _Distance*);   // 0x0086F930
template <class _RandomAccessIter, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIter __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x0086F450
template <class _RandomAccessIter, class _Compare>
void __fastcall sort_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x0086FB40

template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _Tp> inline int* __distance_type(const _Tp*) { return (int*)0; }

template <class _RandomAccessIter, class _Compare>
inline void make_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, __value_type(__first), __distance_type(__first));
}

template <class _RandomAccessIter, class _Tp, class _Compare, class _Distance>
__forceinline void __pop_heap(_RandomAccessIter __first, _RandomAccessIter __last, _RandomAccessIter __result, _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, __distance_type(__first));
    sort_heap(__first, __middle, __comp);
}
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<EExpressionType, CExpressionSet::CEntry> TExpressionPair;
typedef CKeyPairCompareLess<EExpressionType, CExpressionSet::CEntry> TExpressionLess;
template void __fastcall std::__partial_sort<TExpressionPair*, TExpressionPair, TExpressionLess>(TExpressionPair*, TExpressionPair*, TExpressionPair*, TExpressionPair*, TExpressionLess);
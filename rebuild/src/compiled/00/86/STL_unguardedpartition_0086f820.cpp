// _STL::__unguarded_partition(pair<EExpressionType, CExpressionSet::CEntry>* first, pair<EExpressionType, CEntry>* last, pair<EExpressionType, CEntry> pivot, CKeyPairCompareLess<EExpressionType, CEntry> comp) -- STLport quicksort partition of a creature expression set's vector map: walk both ends inwards past correctly placed keys and swap the strays, return the split point
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086F820, length 120 bytes, `ret 10h`.
// Callers: the map's __introsort_loop 0x008700A0 (landed under the descriptive SGI::IntrosortLoop_Element12; 0x00870140).
// Callees: none (iter_swap and the key compare are inline).
// Label: catalog "std::introsort_partition_3way_12byte" is descriptive; STLport 4.x `__unguarded_partition` verbatim.
// Element: pair<EExpressionType, CExpressionSet::CEntry> (12 bytes, signed key), identified as in this chunk's 0x0086F090.
// Shape: as this chunk's 0x0086B3B0 (the 16-byte skeletal-morph twin): /Gr algorithm TU (`__fastcall` template: first in
//   ecx, last in edx, the 12-byte pivot and the empty comparator on the stack); the pivot key re-read at each outer
//   pass; iter_swap's temporary spills its two value words into the pivot's dead second-member slots.
enum EExpressionType { EXPRESSION_TYPE_NONE = 0 };

class CExpressionSet {
public:
    class CEntry {                                          // Ego_r.pdb, 8 bytes of POD
    public:
        EExpressionType Type;                               // +0x00
        long            AnimIndex;                          // +0x04
    };
};

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x00
    _T2 second;                                             // +0x04
    pair(const pair& __o) : first(__o.first), second(__o.second) { }   // STLport's explicit pair copy constructor
};

template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }

template <class _ForwardIter1, class _ForwardIter2, class _Tp>
inline void __iter_swap(_ForwardIter1 __a, _ForwardIter2 __b, _Tp*)
{
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}
template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __a, _ForwardIter2 __b)
{
    __iter_swap(__a, __b, __value_type(__a));
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

typedef _STL::pair<EExpressionType, CExpressionSet::CEntry> TExpressionPair;
typedef CKeyPairCompareLess<EExpressionType, CExpressionSet::CEntry> TExpressionLess;
template TExpressionPair* __fastcall _STL::__unguarded_partition<TExpressionPair*, TExpressionPair, TExpressionLess>(TExpressionPair*, TExpressionPair*, TExpressionPair, TExpressionLess);
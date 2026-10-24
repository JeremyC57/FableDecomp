// std::__introsort_loop(pair<unsigned long,C3DVector>* first, pair<unsigned long,C3DVector>* last, pair<unsigned long,C3DVector>*, int depth_limit, CParticleUpdateCCompareVector<unsigned long> comp) -- STLport introsort loop for the particle vector-parameter map: median-of-three partition and recurse on the right part while more than 16 pairs remain, falling back to partial_sort when the depth limit runs out
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076BE30, 210 bytes, /Gr `__fastcall` `ret 0Ch` (first in ecx -> edi, last in
//   edx -> ebx; the value-type tag pointer, the depth limit and the empty comparator on the stack).
// Callees: __unguarded_partition @ 0x00769AC0 (first/last in ecx/edx, the 16-byte pivot by value, the comparator),
//   itself, __partial_sort @ 0x0076B530 (first, middle, last, (T*)0, comp).
// Caller: STLport sort 0x0076C000 (landed: `sort<pair<unsigned long,C3DVector>*>` reached from
//   NParticleEngine::CParticleUpdateParameters<unsigned long,unsigned long>::Update 0x0076C750, VectorParams.Sort()).
// Element: std::pair<unsigned long, C3DVector> (16 bytes; the key compared UNSIGNED: `jae`/`jb`), comparator
//   CParticleUpdateCCompareVector<unsigned long> (FableWin.pdb), empty: `a.first < b.first`.
// Label correction: catalogued as `Sort_IntrosortPhase_4UintElem`; the body is STLport `__introsort_loop`.
// Shape: STLport `while (last - first > __stl_threshold) { if (depth_limit == 0) { partial_sort(first, last, last, comp);
//   return; } --depth_limit; cut = __unguarded_partition(first, last, _Tp(__median(*first, *(first + (last - first) / 2),
//   *(last - 1), comp)), comp); __introsort_loop(cut, last, (_Tp*)0, depth_limit, comp); last = cut; }` -- `__median` is
//   inline here (the key compares are visible), the pivot is built in the argument area by STLport's explicit pair copy
//   constructor, and the decremented depth is stored back to its argument slot.
class C3DVector {                                       // user memberwise copy: four in-order dword moves for the pivot
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

namespace std {

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};

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
                                                   _Compare __comp);                                   // 0x00769AC0
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp);                                                     // 0x0076B530

template <class _RandomAccessIter, class _Tp, class _Compare>
inline void __partial_sort_front(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last,
                                 _Tp*, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, (_Tp*)0, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit,
                                 _Compare __comp)
{
    while (__last - __first > __stl_threshold) {
        if (__depth_limit == 0) {
            __partial_sort_front(__first, __last, __last, (_Tp*)0, __comp);
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

}

namespace NParticleEngine {
template <class TOwner, class TValue> class CParticleUpdateParameters {
public:
    template <class TKey> struct CParticleUpdateCCompareVector {
        bool operator()(const std::pair<TKey, C3DVector>& a, const std::pair<TKey, C3DVector>& b) const { return a.first < b.first; }
    };
};
}

typedef std::pair<unsigned long, C3DVector> CKeyedVector;
typedef NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>::CParticleUpdateCCompareVector<unsigned long> CCompareVector;

template void __fastcall std::__introsort_loop<CKeyedVector*, CKeyedVector, int, CCompareVector>(
    CKeyedVector*, CKeyedVector*, CKeyedVector*, int, CCompareVector);
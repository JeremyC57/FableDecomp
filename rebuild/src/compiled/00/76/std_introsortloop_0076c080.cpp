// std::__introsort_loop(pair<unsigned long,CParticleUpdateParamDesc<ulong,ulong> >* first, pair<..>* last, pair<..>*, int depth_limit, CParticleUpdateCCompareParamDesc<unsigned long> comp) -- STLport introsort loop for the particle update-parameter map: median-of-three partition and recurse on the right part while more than 16 pairs remain, falling back to partial_sort when the depth limit runs out
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076C080, 213 bytes, /Gr `__fastcall` `ret 0Ch` (first in ecx -> ebx, last in
//   edx -> edi; the value-type tag pointer, the depth limit and the empty comparator on the stack).
// Callees: __unguarded_partition @ 0x0076AA30 (first/last in ecx/edx, the 16-byte pivot by value, the comparator),
//   itself, __partial_sort @ 0x0076BBC0 (this chunk; first, middle, last, (T*)0, comp).
// Caller: STLport sort 0x0076C1A0 (landed: reached from NParticleEngine::CParticleUpdateParameters<unsigned long,
//   unsigned long>::Update 0x0076C750, UpdateParams.Sort()).
// Element: std::pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long,unsigned long> > (16 bytes:
//   the key compared UNSIGNED, then the CBase-derived descriptor -- vtable 0x0125FEE4, Callback, Index -- the model of the
//   landed sort_heap 0x0076B650 / __adjust_heap 0x0076A4E0); comparator CParticleUpdateCCompareParamDesc<unsigned long>.
// Label correction: catalogued as `Sort_IntrosortPhase_Sorted_4Uint`; the body is STLport `__introsort_loop`.
// Shape: as the C3DVector twin 0x0076BE30: inline `__median`, the pivot `_Tp(__median(..))` built in the argument area by
//   STLport's explicit pair copy constructor (the descriptor's implicit copy stores its vtable immediate at +4 and copies
//   the two members), the decremented depth stored back to its argument slot.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

namespace NParticleEngine {
template <class TOwner, class TValue> class CParticleUpdateParamCallback;

template <class TOwner, class TValue> class CParticleUpdateParamDesc : public CBaseClass {   // vtable 0x0125FEE4, 12 bytes
public:
    CParticleUpdateParamCallback<TOwner, TValue>* Callback;   // +0x04
    unsigned long                                 Index;      // +0x08
};
}

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
                                                   _Compare __comp);                                   // 0x0076AA30
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp);                                                     // 0x0076BBC0

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
    template <class TKey> struct CParticleUpdateCCompareParamDesc {
        bool operator()(const std::pair<TKey, CParticleUpdateParamDesc<TOwner, TValue> >& a,
                        const std::pair<TKey, CParticleUpdateParamDesc<TOwner, TValue> >& b) const { return a.first < b.first; }
    };
};
}

typedef std::pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> > CKeyedDesc;
typedef NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>::CParticleUpdateCCompareParamDesc<unsigned long> CCompareParamDesc;

template void __fastcall std::__introsort_loop<CKeyedDesc*, CKeyedDesc, int, CCompareParamDesc>(
    CKeyedDesc*, CKeyedDesc*, CKeyedDesc*, int, CCompareParamDesc);
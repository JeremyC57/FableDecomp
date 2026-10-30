// std::__partial_sort(pair<unsigned long,CParticleUpdateParamDesc<ulong,ulong> >* first, pair<..>* middle, pair<..>* last, pair<..>*, CParticleUpdateCCompareParamDesc<unsigned long> comp) -- STLport partial sort for the particle update-parameter map: heapify [first, middle), pop every smaller later element into the heap, then sort the heap
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076BBC0, 167 bytes (+ int3 padding), /Gr `__fastcall` `ret 0Ch` (first in
//   ecx -> edi, middle in edx -> ebx; last, the value-type tag pointer and the empty comparator on the stack).
// Callees: __make_heap(first, last, comp, (T*)0, (int*)0) @ 0x0076ABE0, __adjust_heap(first, hole, len, value, comp)
//   @ 0x0076A4E0 (landed), CBaseClass::~CBaseClass @ 0x0099A2E0 (destroys the popped value), sort_heap(first, last,
//   comp) @ 0x0076B650 (landed).
// Caller: __introsort_loop 0x0076C080 (this chunk) when the depth limit runs out.
// Element: std::pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long,unsigned long> > (16 bytes,
//   key compared UNSIGNED; the CBase-derived descriptor has vtable 0x0125FEE4) -- the landed 0x0076B650 model.
// Label correction: catalogued as `Partial_Sort_Pair16_TextureMorph`; the element is the particle descriptor pair.
// Shape: STLport `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, _Tp(*i), comp, __DISTANCE_TYPE(first)); sort_heap(first, middle, comp);` with make_heap
//   forwarding to the out-of-line __make_heap and __pop_heap inline: its by-value `_Tp(*i)` lives in the frame (the
//   implicit descriptor copy stores the vtable immediate), `*result = *first` is the implicit pair assignment (no vptr),
//   the __adjust_heap value is built in the argument area by STLport's explicit pair copy constructor, and the frame
//   copy's destructor reduces to the base destructor call.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

namespace NParticleEngine {
template <class TOwner, class TValue> class CParticleUpdateParamCallback;

template <class TOwner, class TValue> class CParticleUpdateParamDesc : public CBaseClass {   // vtable 0x0125FEE4, 12 bytes
public:
    CParticleUpdateParamCallback<TOwner, TValue>* Callback;   // +0x04
    unsigned long                                 Index;      // +0x08
};

template <class TOwner, class TValue> class CParticleUpdateParameters {
public:
    template <class TKey> struct CParticleUpdateCCompareParamDesc;
};
}

namespace std {

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*,
                            _Distance*);                                                       // 0x0076ABE0
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp);                                               // 0x0076A4E0
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x0076B650

template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _Tp> inline int* __distance_type(const _Tp*) { return (int*)0; }

template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, __value_type(__first), __distance_type(__first));
}

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                       _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, __distance_type(__first));
    sort_heap(__first, __middle, __comp);
}

}

namespace NParticleEngine {
template <> template <> struct CParticleUpdateParameters<unsigned long, unsigned long>::CParticleUpdateCCompareParamDesc<unsigned long> {
    bool operator()(const std::pair<unsigned long, CParticleUpdateParamDesc<unsigned long, unsigned long> >& a,
                    const std::pair<unsigned long, CParticleUpdateParamDesc<unsigned long, unsigned long> >& b) const
    {
        return a.first < b.first;
    }
};
}

typedef std::pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> > CKeyedDesc;
typedef NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>::CParticleUpdateCCompareParamDesc<unsigned long> CCompareParamDesc;

template void __fastcall std::__partial_sort<CKeyedDesc*, CKeyedDesc, CCompareParamDesc>(
    CKeyedDesc*, CKeyedDesc*, CKeyedDesc*, CKeyedDesc*, CCompareParamDesc);
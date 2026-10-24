// std::__partial_sort(CDegradableInfo* first, CDegradableInfo* middle, CDegradableInfo* last, CDegradableInfo*, CIsHealth1GTHealth2 comp) -- STLport partial_sort core over CDegradableDef's degradation stages: heap the first part, push every later stage that orders first through it, then sort the heap
// Leaf: ??$__partial_sort@PAVCDegradableInfo@@
//
// Retail 0x0067F150, 168 bytes, `ret 0Ch` (/Gr: first in ecx, middle in edx; last, the _Tp* tag and the empty
//   comparator on the stack); frame `sub esp, 20h` = the 28-byte value copy + the spilled `middle`.
// Callees: std::__make_heap<CDegradableInfo*, CIsHealth1GTHealth2, CDegradableInfo, int> @ 0x0067EAD0 (`ret 0Ch`: comp
//   and the two null tags), std::__adjust_heap<..> @ 0x0067E8F0 (`ret 24h`), std::sort_heap<..> @ 0x0067F040 (`ret 4`;
//   catalog "_Sort_heap<CDegradableInfo*,CDegradableDef::CIsHealth1GTHealth2>").
// Callers: 0x0067F20C / 0x0067F38F (partial_sort and __introsort_loop of CDegradableDef's vector<CDegradableInfo>).
// Layout: Ego_r.pdb CDegradableInfo (28 bytes) = { float HealthPercentage; CEngineGraphic EngineGraphic (+4, 14 packed
//   bytes); long SmashParticleEmitter (+0x14); bool BlocksNavigation (+0x18); }; no engine header exists.
// Shape: STLport _algo.c `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, _Tp(*i), comp, _Distance*); sort_heap(first, middle, comp);` -- make_heap is the
//   inline forwarder to the out-of-line __make_heap, __pop_heap is inline (__forceinline): value = *i, *i = *first,
//   __adjust_heap(first, 0, middle - first, value, comp).  /Gr TU written as explicit __fastcall.
// Label: catalog "swap<CDebugSphere>" is propagated.
#pragma pack(push, 1)
class CEngineGraphic {                                      // 14 bytes (Ego_r.pdb)
public:
    unsigned long BankIndex;                                // +0x00
    float         AnimStep;                                 // +0x04
    float         RenderSizeX;                              // +0x08
    bool          AdditiveAlpha;                            // +0x0c
    unsigned char Type;                                     // +0x0d
};
#pragma pack(pop)

class CDegradableInfo {                                     // 28 bytes
public:
    float          HealthPercentage;                        // +0x00
    CEngineGraphic EngineGraphic;                           // +0x04
    long           SmashParticleEmitter;                    // +0x14
    bool           BlocksNavigation;                        // +0x18
};

class CDegradableDef {
public:
    class CIsHealth1GTHealth2 {                             // stages sorted by descending health
    public:
        bool operator()(const CDegradableInfo& a, const CDegradableInfo& b) const
        {
            return a.HealthPercentage > b.HealthPercentage;
        }
    };
};

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp);                                 // 0x0067E8F0
template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*,
                            _Distance*);                                        // 0x0067EAD0
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x0067F040

template <class _Tp> inline _Tp* __value_type(_Tp* const&) { return (_Tp*)0; }
template <class _Tp> inline int* __distance_type(_Tp* const&) { return (int*)0; }

template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, __value_type(__first), __distance_type(__first));
}

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
__forceinline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                              _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template <class _RandomAccessIterator, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIterator __first, _RandomAccessIterator __middle, _RandomAccessIterator __last,
                               _Tp*, _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIterator __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, __distance_type(__first));
    sort_heap(__first, __middle, __comp);
}
}

template void __fastcall std::__partial_sort<CDegradableInfo*, CDegradableInfo, CDegradableDef::CIsHealth1GTHealth2>(
    CDegradableInfo*, CDegradableInfo*, CDegradableInfo*, CDegradableInfo*, CDegradableDef::CIsHealth1GTHealth2);
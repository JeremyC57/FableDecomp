// std::__unguarded_partition(CThing** first, CThing** last, CThing* pivot, ThingCompare comp) -- STLport Hoare partition around pivot (no bounds checks: the median-of-three guarantees sentinels); returns the cut
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00932490, 104 bytes, `ret 8` (first in ecx, last in edx; pivot and comp on the stack).
// Name: STLport `__unguarded_partition<CThing**, CThing*, bool (__fastcall*)(CThing*, CThing*)>`; caller __introsort_loop
//   0x00932886. The catalog's "Sort_Partition_Predicate_Forward" is descriptive.
// Element: CThing* sorted under a plain `bool (__fastcall*)(CThing*, CThing*)` function pointer -- the sort of
//   CAIStateGroup_MinionReactToCriminal::Update (0x0093150A: `sort(first, last, &0x00930D90)`, a file-local compare whose
//   body is `a->GetPosition().Z < b->GetPosition().Z` through CThing::GetPosition 0x004C73D0). The pointee is only ever
//   handed to GetPosition, so CThing (or a class with CThing at offset 0) is all the bytes show. The catalog's
//   CThingBuilding / CIsHouseLargerThan labels are propagated from the Ego_r.pdb Dinkumware instantiation of another TU.
// /Gr algorithm TU: the first two pointer/int arguments in ecx/edx (the element value and the comparator pointer are
//   register candidates), the rest on the stack; the comparator is called with both elements in ecx/edx.
// Landed siblings in the TU: __median 0x009322D0, __push_heap 0x00932370, sort 0x009328D0.
// Shape: STLport `for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last)) --last; if (!(first <
//   last)) return first; iter_swap(first, last); ++first; }` -- comp in ebx, pivot in ebp, iter_swap/swap inlined to the
//   two-register exchange.
#include <stddef.h>
extern "C" void* __cdecl memmove(void* dst, const void* src, size_t n);   // 0x00BFEAE6

class CThing;
typedef bool (__fastcall* ThingCompare)(CThing* a, CThing* b);   // e.g. the height compare 0x00930D90

namespace std {
template <class _Tp>
inline void swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    swap(*__i1, *__i2);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot,
                                                   _Compare __comp)
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

template CThing** __fastcall __unguarded_partition(CThing**, CThing**, CThing*, ThingCompare);
}
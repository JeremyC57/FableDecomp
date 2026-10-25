// std::__introsort_loop(CThing** first, CThing** last, CThing**, int depth_limit, ThingCompare comp) -- STLport introsort recursion: partition around the median of three, recurse on the right part, loop on the left; partial-sort fallback when the depth runs out
// Leaf: ??$__introsort_loop@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009327F0, 217 bytes (224 with padding), `ret 0Ch` (first in ecx, last in edx; the value-type pointer, the depth limit and comp on the stack).
// Name: STLport `__introsort_loop<CThing**, CThing*, int, bool (__fastcall*)(CThing*, CThing*)>`; callers sort 0x00932908
//   and itself. Callees: __unguarded_partition 0x00932490, __partial_sort 0x00932770. The catalog's "std_sort_QuickSort"
//   is descriptive.
// Element: CThing* sorted under a plain `bool (__fastcall*)(CThing*, CThing*)` function pointer -- the sort of
//   CAIStateGroup_MinionReactToCriminal::Update (0x0093150A: `sort(first, last, &0x00930D90)`, a file-local compare whose
//   body is `a->GetPosition().Z < b->GetPosition().Z` through CThing::GetPosition 0x004C73D0). The pointee is only ever
//   handed to GetPosition, so CThing (or a class with CThing at offset 0) is all the bytes show. The catalog's
//   CThingBuilding / CIsHouseLargerThan labels are propagated from the Ego_r.pdb Dinkumware instantiation of another TU.
// /Gr algorithm TU: the first two pointer/int arguments in ecx/edx (the element value and the comparator pointer are
//   register candidates), the rest on the stack; the comparator is called with both elements in ecx/edx.
// Landed siblings in the TU: __median 0x009322D0, __push_heap 0x00932370, sort 0x009328D0.
// Shape: STLport `while (last - first > __stl_threshold) { if (depth_limit == 0) { partial_sort(first, last, last, comp);
//   return; } --depth_limit; cut = __unguarded_partition(first, last, _Tp(__median(*first, *(first + (last - first) / 2),
//   *(last - 1), comp)), comp); __introsort_loop(cut, last, (_Tp*)0, depth_limit, comp); last = cut; }` -- here __median
//   (an `inline` returning `const _Tp&`) IS inlined: the five compares select a pointer to the chosen element in esi
//   (the middle element's address parked in the dead comparator slot), whose value is pushed as the pivot.
#include <stddef.h>
extern "C" void* __cdecl memmove(void* dst, const void* src, size_t n);   // 0x00BFEAE6

class CThing;
typedef bool (__fastcall* ThingCompare)(CThing* a, CThing* b);   // e.g. the height compare 0x00930D90

namespace std {
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
                                                   _Compare __comp);   // 0x00932490
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp);   // 0x00932770
template <class _RandomAccessIter, class _Compare>
inline void partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, (CThing**)0, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp)
{
    _RandomAccessIter __cut;
    while (__last - __first > 16) {
        if (__depth_limit == 0) {
            partial_sort(__first, __last, __last, __comp);
            return;
        }
        --__depth_limit;
        __cut = __unguarded_partition(__first, __last,
                                      _Tp(__median(*__first, *(__first + (__last - __first) / 2), *(__last - 1), __comp)),
                                      __comp);
        __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp);
        __last = __cut;
    }
}

template void __fastcall __introsort_loop(CThing**, CThing**, CThing**, int, ThingCompare);
}